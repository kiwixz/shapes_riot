#include "utils/os.h"

#include <array>
#include <cstdlib>
#include <mutex>

#include "utils/bit_cast.h"
#include "utils/c_ptr.h"
#include "utils/exception.h"
#include "utils/global_handle.h"

#ifdef _WIN32
#    include <ShlObj.h>
#    include <dbghelp.h>

#    define WIN32_LEAN_AND_MEAN
#    define NOMINMAX
#    include <Windows.h>
#else
#    include <execinfo.h>
#    include <pthread.h>
#    include <pwd.h>
#    include <sys/syscall.h>
#    include <unistd.h>
#endif

namespace utils {

#ifdef _WIN32

namespace {

template <typename T>
void local_free(T native)
{
    if (LocalFree(native))
        throw MAKE_EXCEPTION("could not free: {:#08x}", static_cast<unsigned>(GetLastError()));
}

template <typename T>
void co_task_mem_free(T native)
{
    return CoTaskMemFree(native);
}

std::string from_native_string(std::wstring_view native)
{
    std::string r;
    r.resize(WideCharToMultiByte(CP_UTF8, 0,
                                 native.data(), static_cast<int>(native.size()),
                                 nullptr, 0,
                                 nullptr, nullptr));
    if (!WideCharToMultiByte(CP_UTF8, 0,
                             native.data(), static_cast<int>(native.size()),
                             r.data(), static_cast<int>(r.size()),
                             nullptr, nullptr))
        throw MAKE_EXCEPTION("could not convert string from native: {:#08x}", static_cast<unsigned>(GetLastError()));
    return r;
}

std::wstring to_native_string(std::string_view native)
{
    std::wstring r;
    r.resize(MultiByteToWideChar(CP_UTF8, 0,
                                 native.data(), static_cast<int>(native.size()),
                                 nullptr, 0));
    if (!MultiByteToWideChar(CP_UTF8, 0,
                             native.data(), static_cast<int>(native.size()),
                             r.data(), static_cast<int>(r.size())))
        throw MAKE_EXCEPTION("could not convert string to native: {:#08x}", static_cast<unsigned>(GetLastError()));
    return r;
}

}  // namespace


std::filesystem::path app_directory(std::string_view app_name)
{
    auto base = utils::make_c_ptr<wchar_t[], &co_task_mem_free<PWSTR>>([&](PWSTR& ptr) {
        HRESULT r = SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &ptr);
        if (FAILED(r))
            throw MAKE_EXCEPTION("could not get appdata: {:#08x}", static_cast<unsigned>(r));
    });

    std::filesystem::path path = std::filesystem::path{base.get()} / "kiwixz" / app_name;
    std::filesystem::create_directories(path);
    return path;
}

ThreadId thread_id()
{
    thread_local const ThreadId id = GetCurrentThreadId();
    return id;
}

std::string thread_name()
{
    auto name = utils::make_c_ptr<wchar_t[], &local_free<PWSTR>>([&](PWSTR& ptr) {
        HRESULT r = GetThreadDescription(GetCurrentThread(), &ptr);
        if (FAILED(r))
            throw MAKE_EXCEPTION("could not get thread name: {:#08x}", static_cast<unsigned>(r));
    });
    return from_native_string(name.get());
}

void set_thread_name(const std::string& name)
{
    SetThreadDescription(GetCurrentThread(), to_native_string(name).c_str());
}

std::vector<StackFrame> stacktrace()
{
#    if 1

    struct DbgHandle {
        DbgHandle()
        {
            SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
            if (!SymInitialize(GetCurrentProcess(), nullptr, true))
                throw MAKE_EXCEPTION("could not initialize dbghelp");
        };

        ~DbgHandle()
        {
            SymCleanup(GetCurrentProcess());
        };

        DbgHandle(const DbgHandle&) = delete;
        DbgHandle& operator=(const DbgHandle&) = delete;
        DbgHandle(DbgHandle&&) noexcept = delete;
        DbgHandle& operator=(DbgHandle&&) noexcept = delete;

        std::mutex mutex;
    };

    static DbgHandle dbg_handle;

    std::lock_guard lock{dbg_handle.mutex};


    CONTEXT context = {};
    context.ContextFlags = CONTEXT_FULL;
    RtlCaptureContext(&context);

    bool first = true;
    std::vector<StackFrame> r;

    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    STACKFRAME frame = {};
    frame.AddrPC.Offset = context.Rip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context.Rbp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context.Rsp;
    frame.AddrStack.Mode = AddrModeFlat;

    while (StackWalk(IMAGE_FILE_MACHINE_AMD64, process, thread, &frame, &context,
                     nullptr, SymFunctionTableAccess, SymGetModuleBase, nullptr)) {
        StackFrame f;
        f.address = bit_cast<void*>(frame.AddrPC.Offset);

        f.module = [&] {
            DWORD64 module_base = SymGetModuleBase(process, frame.AddrPC.Offset);
            if (module_base == 0)
                return fmt::format("?unknown_module{}", GetLastError());

            std::array<char, MAX_PATH> buf;
            size_t path_size = GetModuleFileName(bit_cast<HMODULE>(module_base), buf.data(), static_cast<DWORD>(buf.size()));
            if (path_size == 0)
                return fmt::format("?unknown_module_file{}", GetLastError());

            std::string_view module_path{buf.data(), path_size};

            size_t idx = module_path.rfind('\\');
            return std::string{idx == std::string_view::npos ? module_path : module_path.substr(idx + 1)};
        }();

        DWORD64 offset = 0;

        char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
        PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
        symbol->SizeOfStruct = (sizeof IMAGEHLP_SYMBOL) + 255;
        symbol->MaxNameLength = 254;

        if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &offset, symbol)) {
            f.function = symbol->Name;
        }
        else {
            DWORD error = GetLastError();
            f.function = fmt::format("?unknown_function{}", error);
        }

        IMAGEHLP_LINE line;
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

        DWORD offset_ln = 0;
        if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset_ln, &line)) {
            f.file = line.FileName;
            f.line = line.LineNumber;
        }
        else {
            f.line = 0;
        }

        if (!first) {
            r.push_back(f);
        }
        first = false;
    }

    return r;

#    endif
    return {};
}

#else

namespace {

void free_charpp(char** ptr)
{
    return free(ptr);
}

}  // namespace

std::filesystem::path app_directory(std::string_view app_name)
{
    const char* base = std::getenv("HOME");
    std::vector<char> buffer;
    if (!base) {
        buffer.resize(4096);
        passwd pw;
        passwd* result;
        while (getpwuid_r(getuid(), &pw, buffer.data(), buffer.size(), &result) == ERANGE)
            buffer.resize(buffer.size() * 2);
        if (!result)
            throw MAKE_EXCEPTION("could not get home nor passwd of user");
        base = result->pw_dir;  // pointee is in buffer
        if (!base)
            throw MAKE_EXCEPTION("user has no home");
    }

    std::filesystem::path path = std::filesystem::path{base} / ".kiwixz" / app_name;
    std::filesystem::create_directories(path);
    return path;
}

ThreadId thread_id()
{
    thread_local const auto id = static_cast<ThreadId>(syscall(SYS_gettid));
    return id;
}

std::string thread_name()
{
    std::string r;
    r.resize(16);
    pthread_getname_np(pthread_self(), r.data(), r.size());
    r.resize(r.find('\0'));
    return r;
}

void set_thread_name(const std::string& name)
{
    pthread_setname_np(pthread_self(), name.c_str());
}

std::vector<std::string> stacktrace()
{
    std::array<void*, 100> pointers;
    int nr_frames = backtrace(pointers.data(), pointers.size());

    auto r = utils::make_c_ptr<char*, free_charpp>(backtrace_symbols(pointers.data(), nr_frames));
    if (!r)
        throw MAKE_EXCEPTION("could not get stacktrace symbols");

    std::vector<std::string> r;
    r.resize(nr_frames);
    for (int i = 0; i < nr_frames; ++i)
        r[i] = r.get()[i];
    return r;
}

#endif

}  // namespace utils

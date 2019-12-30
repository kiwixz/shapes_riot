#include "utils/os.h"

#include "utils/c_ptr.h"
#include "utils/exception.h"

#ifdef _WIN32
#    include <ShlObj.h>

#    define WIN32_LEAN_AND_MEAN
#    define NOMINMAX
#    include <Windows.h>
#else
#    include <cstdlib>

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

std::vector<std::string> stacktrace()
{
    return {};
}

#else

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

    auto free_charss = [](char** ptr) { return free(ptr); };
    auto frames = utils::make_c_ptr<char*, free_charss>(backtrace_symbols(pointers.data(), nr_frames));
    if (!frames)
        throw MAKE_EXCEPTION("could not get stacktrace symbols");

    std::vector<std::string> r;
    r.resize(nr_frames);
    for (int i = 0; i < nr_frames; ++i)
        r[i] = frames.get()[i];
    return r;
}

#endif

}  // namespace utils

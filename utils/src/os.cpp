#include "utils/os.h"

#include "utils/c_ptr.h"
#include "utils/exception.h"

#ifdef _WIN32
#    define WIN32_LEAN_AND_MEAN
#    define NOMINMAX
#    include <Windows.h>
#else
#    include <pthread.h>
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

#else

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

#endif

}  // namespace utils

#pragma once

#ifdef _WIN32
#    define WIN32_LEAN_AND_MEAN
#    define NOMINMAX
#    include <windows.h>
#else
#    include <sys/syscall.h>
#endif

namespace utils {

#ifdef _WIN32

using ThreadId = DWORD;

ThreadId thread_id()
{
    thread_local const ThreadId id = GetCurrentThreadId();
    return id;
}

#else

using ThreadId = pid_t;

ThreadId thread_id()
{
    thread_local const ThreadId id = static_cast<ThreadId>(syscall(SYS_gettid));
    return id;
}

#endif

}  // namespace utils

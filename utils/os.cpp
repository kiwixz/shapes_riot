#include "utils/os.h"

#ifndef _WIN32
#    include <sys/syscall.h>
#endif

namespace utils {

#ifdef _WIN32

ThreadId thread_id()
{
    thread_local const ThreadId id = GetCurrentThreadId();
    return id;
}

#else

ThreadId thread_id()
{
    thread_local const auto id = static_cast<ThreadId>(syscall(SYS_gettid));
    return id;
}

#endif

}  // namespace utils

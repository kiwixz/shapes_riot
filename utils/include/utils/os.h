#pragma once

#ifdef _WIN32
#    define WIN32_LEAN_AND_MEAN
#    define NOMINMAX
#    include <windows.h>
#else
#    include <unistd.h>
#endif

namespace utils {

#ifdef _WIN32

using ThreadId = DWORD;

#else

using ThreadId = pid_t;

#endif


[[nodiscard]] ThreadId thread_id();

}  // namespace utils

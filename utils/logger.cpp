#include "utils/logger.h"
#include "utils/os.h"
#include <fmt/color.h>
#include <fmt/format.h>
#include <chrono>
#include <iostream>

namespace utils {

Logger::Logger(std::string tag) :
    tag_{std::move(tag)}
{}

bool Logger::operator()(LogLevel level, std::string_view message) const
{
    if (level < tag_log_level(tag_))
        return false;

    sink(level, message);
    return true;
}

void Logger::sink(LogLevel level, std::string_view message) const
{
    std::chrono::duration<double> time = std::chrono::steady_clock::now().time_since_epoch();
    LogLevelInfo level_info = log_level_info(level);

#if 0  // not yet compiling on fmt 6.0.0
    std::clog << fmt::format(level_info.style, "[{:.6f}][{:>5}][{:x}][{}] {}\n",
                             time.count(), level_info.name, thread_id(), tag_, message);
#else
    std::clog << fmt::vformat(level_info.style, "[{:.6f}][{:>5}][{:x}][{}] {}\n",
                              {fmt::make_format_args(time.count(), level_info.name, thread_id(), tag_, message)});
#endif
}

}  // namespace utils

#pragma once

#include <fmt/format.h>
#include <string_view>

namespace utils {

enum class LogLevel {
    debug,
    info,
    warn,
    error,
    crit,
    none,

#ifdef DEBUG
    default_level = debug,
#else
    default_level = info,
#endif
};


LogLevel global_log_level();
LogLevel tag_log_level(std::string_view tag);

void set_global_log_level(LogLevel level);
void set_tag_log_level(std::string_view tag, LogLevel level);

bool log(LogLevel level, std::string_view tag, std::string_view message);

template <typename... Args>
bool log(LogLevel level, std::string_view tag, std::string_view format, Args... args);


template <typename... Args>
bool log(LogLevel level, std::string_view tag, std::string_view format, Args... args)
{
    return log(level, tag, fmt::format(format, std::forward<Args>(args)...));
}

}  // namespace utils

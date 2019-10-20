#pragma once

#include "utils/exception.h"
#include <fmt/color.h>
#include <string_view>

namespace utils {

enum class LogLevel {
    debug,
    info,
    warning,
    error,
    critical,
    none,

#ifdef DEBUG
    default_level = debug,
#else
    default_level = info,
#endif
};


struct LogLevelInfo {
    std::string_view name;
    fmt::text_style style;
};


constexpr LogLevelInfo log_level_info(LogLevel level);

LogLevel global_log_level();                   ///< Thread safe
LogLevel tag_log_level(std::string_view tag);  ///< Thread safe

void set_global_log_level(LogLevel level);                     ///< Thread safe
void set_tag_log_level(std::string_view tag, LogLevel level);  ///< Thread safe
void reset_tag_log_level(std::string_view tag);                ///< Thread safe


constexpr LogLevelInfo log_level_info(LogLevel level)
{
    switch (level) {
    case LogLevel::debug:
        return {"DEBUG", fmt::fg(fmt::terminal_color::green)};
    case LogLevel::info:
        return { "INFO", {} };
    case LogLevel::warning:
        return {"WARN", fmt::fg(fmt::terminal_color::bright_yellow)};
    case LogLevel::error:
        return {"ERROR", fmt::fg(fmt::terminal_color::bright_red)};
    case LogLevel::critical:
        return {"CRIT", fmt::bg(fmt::terminal_color::red) | fmt::fg(fmt::terminal_color::bright_white)};
    case LogLevel::none:
        return { "NONE", {} };
    }

    throw MAKE_EXCEPTION("log level not handled: {}", level);
}

}  // namespace utils

#include "utils/log.h"
#include "utils/exception.h"
#include <fmt/printf.h>
#include <chrono>
#include <thread>
#include <unordered_map>

namespace utils {

namespace {

constexpr std::string_view log_level_name(LogLevel level)
{
    switch (level) {
    case LogLevel::debug:
        return "DEBUG";
    case LogLevel::info:
        return "INFO";
    case LogLevel::warn:
        return "WARN";
    case LogLevel::error:
        return "ERROR";
    case LogLevel::crit:
        return "CRIT";
    case LogLevel::none:
        return "NONE";
    };

    throw MAKE_EXCEPTION("loglevel not handled: {}", level);
}

LogLevel& global_log_level_static()
{
    static LogLevel level = LogLevel::default_level;
    return level;
}

std::unordered_map<std::string, LogLevel>& tags_log_level_static()
{
    static std::unordered_map<std::string, LogLevel> data;
    return data;
}

}  // namespace


LogLevel global_log_level()
{
    return global_log_level_static();
}

LogLevel tag_log_level(std::string_view tag)
{
    std::unordered_map<std::string, LogLevel>& data = tags_log_level_static();
    auto it = data.find(std::string{tag});
    return (it != data.end() ? it->second : global_log_level());
}

void set_global_log_level(LogLevel level)
{
    global_log_level_static() = level;
}

void set_tag_log_level(std::string_view tag, LogLevel level)
{
    tags_log_level_static().insert_or_assign(std::string{tag}, level);
}

bool log(LogLevel level, std::string_view tag, std::string_view message)
{
    if (level < tag_log_level(tag))
        return false;

    std::chrono::duration<double> time = std::chrono::steady_clock::now().time_since_epoch();

    fmt::print(stderr, "[{:.6f}][{:5}][{:x}] {}: {}\n",
               time.count(), log_level_name(level), 0,
               tag, message);
    return true;
}

}  // namespace utils

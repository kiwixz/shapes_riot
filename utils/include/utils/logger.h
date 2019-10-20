#pragma once

#include <fmt/format.h>
#include <string_view>
#include "utils/logging.h"

namespace utils {

struct Logger
{
    Logger(std::string tag);

    bool operator()(LogLevel level, std::string_view message) const;

    template <typename... Args>
    bool operator()(LogLevel level, std::string_view format, Args... args) const;

private:
    std::string tag_;

    void sink(LogLevel level, std::string_view message) const;
};


template <typename... Args>
bool Logger::operator()(LogLevel level, std::string_view format, Args... args) const
{
    if (level < tag_log_level(tag_))
        return false;

    sink(level, fmt::format(format, std::forward<Args>(args)...));
    return true;
}

}  // namespace utils

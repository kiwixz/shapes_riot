#pragma once

#include "utils/logging.h"
#include "utils/span.h"
#include <fmt/format.h>
#include <string_view>

namespace utils {

struct Logger {
    Logger(std::string tag);

    bool operator()(LogLevel level, std::string_view message) const;

    template <typename... Args>
    bool operator()(LogLevel level, std::string_view format, Args&&... args) const;

    bool operator()(LogLevel level, utils::Span<const std::byte> data, std::string_view title) const;

    template <typename... Args>
    bool operator()(LogLevel level, utils::Span<const std::byte> data, std::string_view format, Args&&... args) const;

private:
    std::string tag_;

    void sink(LogLevel level, std::string_view message) const;
    void sink_hex(LogLevel level, utils::Span<const std::byte> data, std::string_view title) const;
};


template <typename... Args>
bool Logger::operator()(LogLevel level, std::string_view format, Args&&... args) const
{
    if (level < tag_log_level(tag_))
        return false;

    sink(level, fmt::format(format, std::forward<Args>(args)...));
    return true;
}

template <typename... Args>
bool Logger::operator()(LogLevel level, utils::Span<const std::byte> data, std::string_view format, Args&&... args) const
{
    if (level < tag_log_level(tag_))
        return false;

    sink_hex(level, data, fmt::format(format, std::forward<Args>(args)...));
    return true;
}

}  // namespace utils

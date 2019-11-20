#pragma once

#include <fmt/format.h>
#include <stdexcept>
#include <string>
#include <string_view>


#define MAKE_EXCEPTION_XSTR(a) #a
#define MAKE_EXCEPTION_STR(a) MAKE_EXCEPTION_XSTR(a)
#define MAKE_EXCEPTION(...) utils::Exception(__FILE__ ":" MAKE_EXCEPTION_STR(__LINE__), __VA_ARGS__)


namespace utils {

struct [[nodiscard]] Exception : std::runtime_error
{
    explicit Exception(std::string_view what);
    Exception(std::string from, std::string_view what);

    template <typename... Args>
    Exception(std::string from, std::string_view format, Args&&... args);

    [[nodiscard]] const std::string& from() const;

private:
    std::string from_;
};


template <typename... Args>
Exception::Exception(std::string from, std::string_view format, Args&&... args) :
    Exception{std::move(from), fmt::format(format, std::forward<Args>(args)...)}
{}

}  // namespace utils

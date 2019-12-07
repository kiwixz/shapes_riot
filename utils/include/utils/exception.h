#pragma once

#include <fmt/format.h>
#include <stdexcept>
#include <string>
#include <string_view>


#define DETAIL_MAKE_EXCEPTION_XSTR(a) #a
#define DETAIL_MAKE_EXCEPTION_STR(a) DETAIL_MAKE_EXCEPTION_XSTR(a)
#define MAKE_EXCEPTION(...) utils::Exception(__FILE__ ":" DETAIL_MAKE_EXCEPTION_STR(__LINE__), __VA_ARGS__)

#define NOT_IMPLEMENTED throw MAKE_EXCEPTION("not implemented");


namespace utils {

struct Exception : std::runtime_error {
    explicit Exception(std::string_view what);
    Exception(std::string from, std::string_view what);

    template <typename... Args>
    Exception(std::string from, std::string_view format, Args&&... args);

    const std::string& from() const;

private:
    std::string from_;
};


[[noreturn]] void terminate();


template <typename... Args>
Exception::Exception(std::string from, std::string_view format, Args&&... args) :
    Exception{std::move(from), fmt::format(format, std::forward<Args>(args)...)}
{}

}  // namespace utils

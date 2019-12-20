#pragma once

#include <exception>
#include <type_traits>

#include "utils/any_detail.h"

namespace utils {

struct Any {
    Any() = default;

    template <typename T, std::enable_if_t<!std::is_same_v<T, Any>, int> = 0>
    Any(T&& value);

    ~Any();
    Any(const Any& other);
    Any& operator=(const Any& other);
    Any(Any&& other) noexcept;
    Any& operator=(Any&& other) noexcept;

    explicit operator bool() const;

private:
    any_detail::Storage storage_;
    void (*proxy_)(any_detail::ProxyOp op, any_detail::Storage* a, any_detail::Storage* b);

    void destroy();
};


template <typename T, std::enable_if_t<!std::is_same_v<T, Any>, int>>
Any::Any(T&& value)
{
    using Type = std::decay_t<T>;

    storage_ = make_c_ptr(operator new(sizeof(Type)));
    new (storage_.get()) Type{std::move(value)};
    proxy_ = any_detail::proxy<Type>;
}

}  // namespace utils

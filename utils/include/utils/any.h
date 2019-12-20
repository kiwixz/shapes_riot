#pragma once

#include <exception>
#include <type_traits>

#include "utils/any_detail.h"

namespace utils {

struct Any {
    Any() = default;

    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Any>, int> = 0>
    Any(T&& value);

    ~Any();
    Any(const Any& other);
    Any& operator=(const Any& other);
    Any(Any&& other) noexcept;
    Any& operator=(Any&& other) noexcept;

    explicit operator bool() const;

    template <typename T>
    T& as() const;

private:
    any_detail::Storage storage_;
    void (*proxy_)(any_detail::ProxyOp op, any_detail::Storage* a, any_detail::Storage* b);

    void destroy();
};


template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Any>, int>>
Any::Any(T&& value)
{
    using Type = std::decay_t<T>;

    set_c_ptr(storage_, operator new(sizeof(Type)));
    new (storage_.get()) Type{std::forward<T>(value)};
    proxy_ = any_detail::proxy<Type>;
}

template <typename T>
T& Any::as() const
{
    return *std::launder(reinterpret_cast<T*>(storage_.get()));
}

}  // namespace utils

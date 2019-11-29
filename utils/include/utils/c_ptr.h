#pragma once

#include "utils/function_ref.h"
#include "utils/static_function.h"
#include <memory>

namespace utils {

template <typename T, void (*Tdeleter)(T*)>
using CPtr = std::unique_ptr<T, StaticFunction<Tdeleter>>;

template <typename T>
bool set_c_ptr(T& ptr, typename T::pointer native);

template <typename T>
bool set_c_ptr(T& ptr, FunctionRef<bool(typename T::pointer)> setter);


template <typename T>
bool set_c_ptr(T& ptr, typename T::pointer native)
{
    ptr.reset(native);
    return native;
}

template <typename T>
bool set_c_ptr(T& ptr, FunctionRef<bool(typename T::pointer)> setter)
{
    typename T::pointer native = nullptr;
    if (!setter(native))
        return false;

    return set_c_ptr(ptr, native);
}

}  // namespace utils

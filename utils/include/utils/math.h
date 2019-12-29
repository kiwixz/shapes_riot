#pragma once

#include <cmath>

namespace utils {

template <typename Unit, typename Arg>
constexpr Unit ceil(Arg a)
{
    Unit unit_a = static_cast<Unit>(a);
    if (a == static_cast<Arg>(unit_a))
        return unit_a;
    return unit_a + (a > 0 ? 1 : 0);
}

/// Only works with positive numbers.
template <typename T, typename Arg>
constexpr T ceil(Arg a, T unit)
{
    return ceil_div(a, unit) * unit;
}

template <typename T>
constexpr T ceil2(T a)
{
    if (a < 0)
        return -ceil2(-a);
    return static_cast<T>(1u << static_cast<unsigned>(std::log2(a - 1) + 1));
}

/// Only works with positive numbers.
template <typename T>
constexpr T ceil_div(T a, T unit)
{
    return (a + unit - 1) / unit;
}

template <typename T, typename Arg>
constexpr T floor(Arg a)
{
    return static_cast<T>(std::floor(a));
}

template <typename T>
constexpr T pow2(T a)
{
    return a * a;
}

}  // namespace utils

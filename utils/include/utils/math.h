#pragma once

#include <cmath>

namespace utils {

/// Only works with positive numbers.
template <typename T>
[[nodiscard]] constexpr T ceil2(T a);

/// Only works with positive numbers.
template <typename T>
[[nodiscard]] constexpr T div_ceil(T a, T b);

/// Only works with positive numbers.
template <typename T>
[[nodiscard]] constexpr T round_up(T a, T unit);


template <typename T>
constexpr T ceil2(T a)
{
    return static_cast<T>(1u << static_cast<unsigned>(std::log2(a - 1) + 1));
}

template <typename T>
constexpr T div_ceil(T a, T b)
{
    return (a + b - 1) / b;
}

template <typename T>
constexpr T round_up(T a, T unit)
{
    return div_ceil(a, unit) * unit;
}

}  // namespace utils

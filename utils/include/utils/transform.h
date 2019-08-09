#pragma once

#include "utils/vec.h"

namespace utils {

template <typename TElement>
struct Transform2 {
    using Element = TElement;

    utils::Vec2<Element> translation{};
    utils::Vec2<Element> scale{1, 1};
};


using Transform2d = Transform2<double>;
using Transform2f = Transform2<float>;
using Transform2i = Transform2<int>;
using Transform2u = Transform2<unsigned>;


template <typename T>
constexpr Transform2<T> operator-(const Transform2<T>& lhs);

template <typename T>
constexpr Transform2<T>& operator*=(Transform2<T>& lhs, const Transform2<T>& rhs);

template <typename T>
constexpr Transform2<T> operator*(const Transform2<T>& lhs, const Transform2<T>& rhs);

template <typename T>
constexpr Vec2<T>& operator*=(Vec2<T>& lhs, const Transform2<T>& rhs);

template <typename T>
constexpr Vec2<T> operator*(const Vec2<T>& lhs, const Transform2<T>& rhs);


template <typename T>
constexpr Transform2<T> operator-(const Transform2<T>& lhs)
{
    return {-lhs.translation / lhs.scale, utils::Vec2<T>{1, 1} / lhs.scale};
}

template <typename T>
constexpr Transform2<T>& operator*=(Transform2<T>& lhs, const Transform2<T>& rhs)
{
    lhs.translation = lhs.translation * rhs.scale + rhs.translation;
    lhs.scale *= rhs.scale;
    return lhs;
}

template <typename T>
constexpr Transform2<T> operator*(const Transform2<T>& lhs, const Transform2<T>& rhs)
{
    Transform2<T> r = lhs;
    r *= rhs;
    return r;
}

template <typename T>
constexpr Vec2<T>& operator*=(Vec2<T>& lhs, const Transform2<T>& rhs)
{
    lhs *= rhs.scale;
    lhs += rhs.translation;
    return lhs;
}

template <typename T>
constexpr Vec2<T> operator*(const Vec2<T>& lhs, const Transform2<T>& rhs)
{
    Vec2<T> r = lhs;
    r *= rhs;
    return r;
}

}  // namespace utils

#pragma once

#include "utils/vec.h"

namespace utils {

template <typename TElement>
struct Transform2 {
    using Element = TElement;

    utils::Vec2<Element> translation{};
    utils::Vec2<Element> scale{};
};


using Transform2d = Transform2<double>;
using Transform2f = Transform2<float>;
using Transform2i = Transform2<int>;
using Transform2u = Transform2<unsigned>;


template <typename T>
constexpr T& operator*=(T& vec, const Transform2<T>& transform);

template <typename T>
constexpr T operator*(const T& vec, const Transform2<T>& transform);


template <typename T>
constexpr utils::Vec2<T>& operator*=(utils::Vec2<T>& vec, const Transform2<T>& transform)
{
    vec *= transform.scale;
    vec += transform.translation;
    return vec;
}

template <typename T>
constexpr utils::Vec2<T> operator*(const utils::Vec2<T>& vec, const Transform2<T>& transform)
{
    return vec * transform.scale + transform.translation;
}

}  // namespace utils

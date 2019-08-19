#pragma once

#include "utils/matrix.h"
#include "utils/vec.h"

namespace utils {

template <typename TElement>
struct Transform3 {
    using Self = Transform3<TElement>;
    using Element = TElement;

    utils::Vec3<Element> translation{};
    utils::Vec3<Element> scale{utils::Vec3i{1, 1, 1}};

    Transform3<TElement> inverse() const;
    Matrix4<TElement> matrix() const;
};


using Transform3d = Transform3<double>;
using Transform3f = Transform3<float>;
using Transform3i = Transform3<int>;
using Transform3u = Transform3<unsigned>;


template <typename T>
constexpr Transform3<T>& operator*=(Transform3<T>& lhs, const Transform3<T>& rhs);

template <typename T>
constexpr Transform3<T> operator*(const Transform3<T>& lhs, const Transform3<T>& rhs);

template <typename T>
constexpr Vec3<T>& operator*=(Vec3<T>& lhs, const Transform3<T>& rhs);

template <typename T>
constexpr Vec3<T> operator*(const Vec3<T>& lhs, const Transform3<T>& rhs);


template <typename TElement>
Transform3<TElement> Transform3<TElement>::inverse() const
{
    return {-translation / scale, static_cast<Element>(1) / scale};
}

template <typename TElement>
Matrix4<TElement> Transform3<TElement>::matrix() const
{
    return {{scale.x, 0, 0, translation.x,
             0, scale.y, 0, translation.y,
             0, 0, scale.z, translation.z,
             0, 0, 0, 1}};
}


template <typename T>
constexpr Transform3<T>& operator*=(Transform3<T>& lhs, const Transform3<T>& rhs)
{
    lhs.translation = lhs.translation * rhs.scale + rhs.translation;
    lhs.scale *= rhs.scale;
    return lhs;
}

template <typename T>
constexpr Transform3<T> operator*(const Transform3<T>& lhs, const Transform3<T>& rhs)
{
    Transform3<T> r = lhs;
    r *= rhs;
    return r;
}

template <typename T>
constexpr Vec3<T>& operator*=(Vec3<T>& lhs, const Transform3<T>& rhs)
{
    lhs *= rhs.scale;
    lhs += rhs.translation;
    return lhs;
}

template <typename T>
constexpr Vec3<T> operator*(const Vec3<T>& lhs, const Transform3<T>& rhs)
{
    Vec3<T> r = lhs;
    r *= rhs;
    return r;
}

}  // namespace utils

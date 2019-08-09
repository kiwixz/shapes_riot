#pragma once

#include "utils/vec_detail.h"

namespace utils {

template <typename TElement>
struct Vec2 : vec_detail::VecMeta<Vec2, TElement, 2> {
    using Element = TElement;
    static constexpr int size = 2;

    Element x{};
    Element y{};

    constexpr Vec2() = default;
    ~Vec2() = default;
    constexpr Vec2(const Vec2<Element>&) = default;
    constexpr Vec2<Element>& operator=(const Vec2<Element>&) = default;
    constexpr Vec2(Vec2<Element>&&) noexcept = default;
    constexpr Vec2<Element>& operator=(Vec2<Element>&&) noexcept = default;

    constexpr Vec2(Element _x, Element _y) :
        x{_x}, y{_y}
    {}

    template <typename T>
    constexpr explicit Vec2(const Vec2<T>& other)
    {
        *this = other;
    }

    template <typename T>
    constexpr Vec2<Element>& operator=(const Vec2<T>& other)
    {
        x = static_cast<Element>(other.x);
        y = static_cast<Element>(other.y);
        return *this;
    }
};

template <typename TElement>
struct Vec3 : vec_detail::VecMeta<Vec2, TElement, 3> {
    using Element = TElement;
    static constexpr int size = 3;

    Element x{};
    Element y{};
    Element z{};

    constexpr Vec3() = default;
    ~Vec3() = default;
    constexpr Vec3(const Vec3<Element>&) = default;
    constexpr Vec3<Element>& operator=(const Vec3<Element>&) = default;
    constexpr Vec3(Vec3<Element>&&) noexcept = default;
    constexpr Vec3<Element>& operator=(Vec3<Element>&&) noexcept = default;

    constexpr Vec3(Element _x, Element _y, Element _z) :
        x{_x}, y{_y}, z{_z}
    {}

    template <typename T>
    constexpr explicit Vec3(const Vec3<T>& other)
    {
        *this = other;
    }

    template <typename T>
    constexpr Vec3<Element>& operator=(const Vec3<T>& other)
    {
        x = static_cast<Element>(other.x);
        y = static_cast<Element>(other.y);
        z = static_cast<Element>(other.z);
        return *this;
    }
};

template <typename TElement>
struct Vec4 : vec_detail::VecMeta<Vec4, TElement, 4> {
    using Element = TElement;
    static constexpr int size = 4;

    Element x{};
    Element y{};
    Element z{};
    Element w{};

    constexpr Vec4() = default;
    ~Vec4() = default;
    constexpr Vec4(const Vec4<Element>&) = default;
    constexpr Vec4<Element>& operator=(const Vec4<Element>&) = default;
    constexpr Vec4(Vec4<Element>&&) noexcept = default;
    constexpr Vec4<Element>& operator=(Vec4<Element>&&) noexcept = default;

    constexpr Vec4(Element _x, Element _y, Element _z, Element _w) :
        x{_x}, y{_y}, z{_z}, w{_w}
    {}

    template <typename T>
    constexpr explicit Vec4(const Vec4<T>& other)
    {
        *this = other;
    }

    template <typename T>
    constexpr Vec4<Element>& operator=(const Vec4<T>& other)
    {
        x = static_cast<Element>(other.x);
        y = static_cast<Element>(other.y);
        z = static_cast<Element>(other.z);
        w = static_cast<Element>(other.w);
        return *this;
    }
};


using Vec2d = Vec2<double>;
using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned>;

using Vec3d = Vec3<double>;
using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;
using Vec3u = Vec3<unsigned>;

using Vec4d = Vec4<double>;
using Vec4f = Vec4<float>;
using Vec4i = Vec4<int>;
using Vec4u = Vec4<unsigned>;

}  // namespace utils

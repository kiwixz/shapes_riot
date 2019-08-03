#pragma once

namespace utils {

template <typename TElement>
struct Vec2 {
    using Element = TElement;

    Element x;
    Element y;
};


template <typename TElement>
struct Vec4 {
    using Element = TElement;

    Element x;
    Element y;
    Element z;
    Element w;
};


using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

using Vec4i = Vec4<int>;
using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;

}  // namespace utils

#pragma once

#include "utils/vec.h"

namespace gfx {

struct Vertex {
    using Index = unsigned;

    constexpr Vertex(utils::Vec2f pos, utils::Vec4f rgb);
    constexpr Vertex(utils::Vec2f pos, utils::Vec4f rgb, utils::Vec2f uv);

    utils::Vec2f pos;
    utils::Vec4f rgb;
    utils::Vec2f uv{};
};
static_assert(sizeof(Vertex) == sizeof(float) * 8);  // ensures there is no padding


constexpr Vertex::Vertex(utils::Vec2f pos_, utils::Vec4f rgb_) :
    pos{pos_}, rgb{rgb_}
{}

constexpr Vertex::Vertex(utils::Vec2f pos_, utils::Vec4f rgb_, utils::Vec2f uv_) :
    pos{pos_}, rgb{rgb_}, uv{uv_}
{}

}  // namespace gfx

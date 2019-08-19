#pragma once

#include "utils/vec.h"

namespace gfx {

struct Vertex {
    using Index = unsigned;

    constexpr Vertex(utils::Vec4f pos, utils::Vec4f rgb);
    constexpr Vertex(utils::Vec4f pos, utils::Vec2f uv);
    constexpr Vertex(utils::Vec4f pos, utils::Vec4f rgb, utils::Vec2f uv);

    utils::Vec4f pos;
    utils::Vec4f rgb{1.0f, 1.0f, 1.0f, 1.0f};
    utils::Vec2f uv;
};
static_assert(sizeof(Vertex) == sizeof(float) * 10);  // ensures there is no padding


constexpr Vertex::Vertex(utils::Vec4f _pos, utils::Vec4f _rgb) :
    pos{_pos}, rgb{_rgb}
{}

constexpr Vertex::Vertex(utils::Vec4f _pos, utils::Vec2f _uv) :
    pos{_pos}, uv{_uv}
{}

constexpr Vertex::Vertex(utils::Vec4f _pos, utils::Vec4f _rgb, utils::Vec2f _uv) :
    pos{_pos}, rgb{_rgb}, uv{_uv}
{}

}  // namespace gfx

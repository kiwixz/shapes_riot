#include "gfx/draw_list.h"
#include <glad/glad.h>
#include <algorithm>
#include <string_view>

namespace gfx {

utils::Span<const DrawList::ColoredVertex> DrawList::vertices() const
{
    return vertices_;
}

utils::Span<const DrawList::Index> DrawList::indexes() const
{
    return indexes_;
}

void DrawList::push(utils::Span<const ColoredVertex> vertices)
{
    vertices_.reserve(vertices_.size() + vertices.size());
    std::copy(vertices.begin(), vertices.end(), std::back_inserter(vertices_));

    indexes_.reserve(indexes_.size() + vertices.size());
    for (int i = 0; i < static_cast<int>(vertices.size()); ++i)
        indexes_.push_back(next_index_ + i);
    next_index_ += static_cast<int>(vertices.size());
}

void DrawList::push(utils::Span<const ColoredVertex> vertices, utils::Span<const Index> index)
{
    vertices_.reserve(vertices_.size() + vertices.size());
    std::copy(vertices.begin(), vertices.end(), std::back_inserter(vertices_));

    indexes_.reserve(indexes_.size() + index.size());
    Index offset = next_index_;
    for (Index idx : index) {
        indexes_.push_back(offset + idx);
        next_index_ = std::max(next_index_, idx + 1);
    }
}

void DrawList::push(const DrawList& draw_list)
{
    push(draw_list.vertices(), draw_list.indexes());
}

void DrawList::transform(utils::Vec2d translation, utils::Vec2d scale)
{
    for (ColoredVertex& vertex : vertices_) {
        vertex.pos.x = static_cast<float>(vertex.pos.x * scale.x + translation.x);
        vertex.pos.y = static_cast<float>(vertex.pos.y * scale.y + translation.y);
    }
}

}  // namespace gfx

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
    for (const auto& idx : index) {
        indexes_.push_back(next_index_ + idx);
        next_index_ = std::max(next_index_, idx + 1);
    }
}

}  // namespace gfx

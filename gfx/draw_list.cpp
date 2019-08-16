#include "gfx/draw_list.h"
#include <glad/glad.h>
#include <algorithm>
#include <array>
#include <string_view>

namespace gfx {

utils::Span<const Vertex> SubDrawList::vertices() const
{
    return vertices_;
}

utils::Span<const Vertex::Index> SubDrawList::indexes() const
{
    return indexes_;
}

void SubDrawList::push(utils::Span<const Vertex> vertices, utils::Span<const Vertex::Index> indexes)
{
    vertices_.reserve(vertices_.size() + vertices.size());
    std::copy(vertices.begin(), vertices.end(), std::back_inserter(vertices_));

    if (indexes.size() != 0) {
        indexes_.reserve(indexes_.size() + indexes.size());
        Vertex::Index offset = next_index_;
        for (Vertex::Index idx : indexes) {
            idx += offset;
            indexes_.push_back(idx);
            next_index_ = std::max(next_index_, idx + 1);
        }
    }
    else {
        indexes_.reserve(indexes_.size() + vertices.size());
        for (int i = 0; i < static_cast<int>(vertices.size()); ++i)
            indexes_.push_back(next_index_ + i);
        next_index_ += static_cast<int>(vertices.size());
    }
}

void SubDrawList::push(const SubDrawList& draw_list)
{
    push(draw_list.vertices(), draw_list.indexes());
}

void SubDrawList::transform(const utils::Transform2f& transform)
{
    for (Vertex& vertex : vertices_)
        vertex.pos *= transform;
}


DrawList::Iterator DrawList::begin() const
{
    return sub_lists_.begin();
}

DrawList::Iterator DrawList::end() const
{
    return sub_lists_.end();
}

void DrawList::push_triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Texture* texture)
{
    push(std::array<Vertex, 3>{a, b, c}, {}, texture);
}

void DrawList::push_quad(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d, const Texture* texture)
{
    static constexpr std::array<Vertex::Index, 6> indexes = {{0, 1, 3, 1, 3, 2}};
    push(std::array<Vertex, 4>{a, b, c, d}, indexes, texture);
}

void DrawList::push(utils::Span<const Vertex> vertices, utils::Span<const Vertex::Index> indexes, const Texture* texture)
{
    sub_lists_[texture].push(vertices, indexes);
}

void DrawList::push(const DrawList& draw_list)
{
    for (const std::pair<const Texture* const, SubDrawList>& pair : draw_list.sub_lists_)
        sub_lists_[pair.first].push(pair.second);
}

void DrawList::transform(const utils::Transform2f& transform)
{
    for (std::pair<const Texture* const, SubDrawList>& pair : sub_lists_)
        pair.second.transform(transform);
}


DrawList& operator+=(DrawList& lhs, const DrawList& rhs)
{
    lhs.push(rhs);
    return lhs;
}

}  // namespace gfx

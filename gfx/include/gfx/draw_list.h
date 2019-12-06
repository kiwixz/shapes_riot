#pragma once

#include "gfx/texture.h"
#include "gfx/vertex.h"
#include "utils/matrix.h"
#include "utils/span.h"
#include <vector>

namespace gfx {

struct SubDrawList {
    SubDrawList(TextureView texture);

    TextureView texture() const;
    utils::Span<const Vertex> vertices() const;
    utils::Span<const Vertex::Index> indexes() const;

    void push(utils::Span<const Vertex> vertices, utils::Span<const Vertex::Index> indexes = {});
    void push(const SubDrawList& draw_list);
    void transform(const utils::Matrix4f& transform);

private:
    TextureView texture_;
    std::vector<Vertex> vertices_;
    std::vector<Vertex::Index> indexes_;
    Vertex::Index next_index_ = 0;
};


struct DrawList {
    using Iterator = std::vector<SubDrawList>::const_iterator;

    Iterator begin() const;
    Iterator end() const;

    void push_triangle(const Vertex& a, const Vertex& b, const Vertex& c, TextureView texture = {});
    void push_quad(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d, TextureView texture = {});
    void push(utils::Span<const Vertex> vertices, utils::Span<const Vertex::Index> indexes = {}, TextureView texture = {});
    void push(const DrawList& other);
    void transform(const utils::Matrix4f& transform);

private:
    std::vector<SubDrawList> sub_lists_;

    SubDrawList* find_sub_list(TextureView texture);
};


DrawList& operator+=(DrawList& lhs, const DrawList& rhs);


}  // namespace gfx

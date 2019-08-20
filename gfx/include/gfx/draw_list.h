#pragma once

#include "gfx/texture.h"
#include "gfx/vertex.h"
#include "utils/matrix.h"
#include "utils/span.h"
#include <map>
#include <vector>

namespace gfx {

struct SubDrawList {
    [[nodiscard]] utils::Span<const Vertex> vertices() const;
    [[nodiscard]] utils::Span<const Vertex::Index> indexes() const;

    void push(utils::Span<const Vertex> vertices, utils::Span<const Vertex::Index> indexes = {});
    void push(const SubDrawList& draw_list);
    void transform(const utils::Matrix4f& transform);

private:
    std::vector<Vertex> vertices_;
    std::vector<Vertex::Index> indexes_;
    Vertex::Index next_index_ = 0;
};


struct DrawList {
    using Iterator = std::map<const Texture*, SubDrawList>::const_iterator;

    [[nodiscard]] Iterator begin() const;
    [[nodiscard]] Iterator end() const;

    void push_triangle(const Vertex& a, const Vertex& b, const Vertex& c,
                       const Texture* texture = nullptr);
    void push_quad(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d,
                   const Texture* texture = nullptr);
    void push(utils::Span<const Vertex> vertices, utils::Span<const Vertex::Index> indexes = {},
              const Texture* texture = nullptr);
    void push(const DrawList& draw_list);
    void transform(const utils::Matrix4f& transform);

private:
    std::map<const Texture*, SubDrawList> sub_lists_;
};


DrawList& operator+=(DrawList& lhs, const DrawList& rhs);


}  // namespace gfx

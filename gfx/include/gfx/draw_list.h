#pragma once

#include "gl_object.h"
#include "utils/vec.h"
#include <vector>

namespace gfx {

struct DrawList {
    using Index = unsigned;

    struct ColoredVertex {
        utils::Vec2f pos;
        utils::Vec4f color;
    };
    static_assert(sizeof(ColoredVertex) == sizeof(float) * 6);

    DrawList();

    void clear();
    void push(const std::vector<ColoredVertex>& vertices);
    void push(const std::vector<ColoredVertex>& vertices, const std::vector<Index>& indexes);

    void draw();

private:
    std::vector<ColoredVertex> vertices_;
    std::vector<Index> indexes_;
    int max_index_;

    GlBuffers<2> buffers_;
    GlVertexArrays<> vertex_array_;
};

}  // namespace gfx

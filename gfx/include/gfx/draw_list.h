#pragma once

#include "utils/span.h"
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

    utils::Span<const ColoredVertex> vertices() const;
    utils::Span<const Index> indexes() const;

    void push(utils::Span<const ColoredVertex> vertices);
    void push(utils::Span<const ColoredVertex> vertices, utils::Span<const Index> indexes);

private:
    std::vector<ColoredVertex> vertices_;
    std::vector<Index> indexes_;
    Index next_index_ = 0;
};

}  // namespace gfx

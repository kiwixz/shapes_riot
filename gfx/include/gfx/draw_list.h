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

    [[nodiscard]] utils::Span<const ColoredVertex> vertices() const;
    [[nodiscard]] utils::Span<const Index> indexes() const;

    void push(utils::Span<const ColoredVertex> vertices);
    void push(utils::Span<const ColoredVertex> vertices, utils::Span<const Index> indexes);
    void push(const DrawList& draw_list);
    void transform(utils::Vec2d translation, utils::Vec2d scale);

private:
    std::vector<ColoredVertex> vertices_;
    std::vector<Index> indexes_;
    Index next_index_ = 0;
};

}  // namespace gfx

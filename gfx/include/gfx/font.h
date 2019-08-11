#pragma once

#include "gfx/texture.h"
#include "utils/scope_exit.h"
#include "utils/span.h"
#include "utils/vec.h"
#include <cstddef>

namespace gfx {

struct Glyph {
    utils::Vec2f offset;
    utils::Vec2f size;
};


struct Font {
    Font(utils::Span<const std::byte> file);

    [[nodiscard]] utils::ScopeExit bind() const;

private:
    Texture texture_;
    std::unordered_map<uint32_t, Glyph> glyphs_;
};

}  // namespace gfx

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
    using GlyphCode = uint32_t;

    Font(GlyphCode first_glyph, int nr_glyph,
         utils::Span<const std::byte> file, int size_px);

    [[nodiscard]] utils::ScopeExit bind() const;

private:
    Texture texture_;
    std::unordered_map<GlyphCode, Glyph> glyphs_;
};

}  // namespace gfx

#pragma once

#include "gfx/draw_list.h"
#include "gfx/texture.h"
#include "utils/span.h"
#include "utils/vec.h"
#include <cstddef>
#include <unordered_map>

namespace gfx {

struct Glyph {
    utils::Vec2f uv_offset;
    utils::Vec2f uv_size;

    utils::Vec2i size;
    utils::Vec2i bearing;
    int advance;
};


struct Font {
    using GlyphCode = uint32_t;

    Font(GlyphCode first_glyph, int nr_glyphs,
         utils::Span<const std::byte> file, int size_px);

    [[nodiscard]] const Texture& texture() const;

private:
    Texture texture_;
    std::unordered_map<GlyphCode, Glyph> glyphs_;
};

}  // namespace gfx

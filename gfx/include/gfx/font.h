#pragma once

#include "gfx/draw_list.h"
#include "gfx/texture.h"
#include "utils/span.h"
#include "utils/vec.h"
#include <cstddef>
#include <string_view>
#include <unordered_map>

namespace gfx {

struct Glyph {
    utils::Vec2f uv_offset;
    utils::Vec2f uv_size;

    utils::Vec2f size;
    utils::Vec2f bearing;
    float advance;
};


struct Font {
    using GlyphCode = uint32_t;

    Font(GlyphCode first_glyph, int nr_glyphs,
         utils::Span<const std::byte> file, int size_px);

    [[nodiscard]] DrawList draw_text(std::string_view text, utils::Vec3f center, utils::Vec2f size) const;
    [[nodiscard]] DrawList draw_text_linear(std::string_view text, utils::Vec3f& pen, utils::Vec2f size) const;
    [[nodiscard]] const Texture& texture() const;

private:
    Texture texture_;
    std::unordered_map<GlyphCode, Glyph> glyphs_;
};

}  // namespace gfx

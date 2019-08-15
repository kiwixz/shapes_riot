#include "gfx/font.h"
#include "gfx/freetype.h"
#include "utils/exception.h"
#include "utils/math.h"
#include <cmath>

namespace gfx {

Font::Font(GlyphCode first_glyph, int nr_glyphs,
           utils::Span<const std::byte> file, int size_px)
{
    FT_Library freetype;
    if (FT_Error error = FT_Init_FreeType(&freetype))
        throw MAKE_EXCEPTION("could not initialize freetype: error {:#x}", error);
    utils::ScopeExit freetype_dtor{std::bind(&FT_Done_FreeType, freetype)};

    FT_Face face;
    if (FT_Error error = FT_New_Memory_Face(freetype,
                                            reinterpret_cast<const FT_Byte*>(file.data()),
                                            static_cast<FT_Long>(file.size()),
                                            0, &face))
        throw MAKE_EXCEPTION("could not load font face: error {:#x}", error);
    utils::ScopeExit face_dtor{std::bind(&FT_Done_Face, face)};

    if (FT_Error error = FT_Set_Pixel_Sizes(face, 0, size_px))
        throw MAKE_EXCEPTION("could not load set face size: error {:#x}", error);

    static constexpr int texture_size = 512;
    texture_.resize({texture_size, texture_size}, GL_RGBA8);

    utils::Vec2i next_offset{1, 1};
    int next_line_y = 0;
    for (int i = 0; i < nr_glyphs; ++i) {
        GlyphCode code = first_glyph + i;
        if (FT_Error error = FT_Load_Glyph(face, FT_Get_Char_Index(face, code), FT_LOAD_RENDER))
            throw MAKE_EXCEPTION("could not load set face size: error {:#x}", error);

        utils::Vec2i glyph_size{utils::Vec2u{face->glyph->bitmap.width, face->glyph->bitmap.rows}};

        std::vector<uint32_t> pixels(glyph_size.x * glyph_size.y);
        for (int y = 0; y < glyph_size.y; ++y)
            for (int x = 0; x < glyph_size.x; ++x)
                pixels[y * glyph_size.x + x] = (face->glyph->bitmap.buffer[y * face->glyph->bitmap.pitch + x] << 24)
                                               | 0x00ffffff;

        utils::Vec2i offset = next_offset;
        if (offset.x + glyph_size.x >= texture_size)  // go to next line
            offset = {0, next_line_y};
        texture_.update(pixels.data(), offset, glyph_size, GL_BGRA);
        next_line_y = std::max(next_line_y, offset.y + glyph_size.y + 1);
        next_offset = {offset.x + glyph_size.x + 1, offset.y};
    }
}

const Texture& Font::texture() const
{
    return texture_;
}

}  // namespace gfx

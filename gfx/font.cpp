#include "gfx/font.h"
#include "gfx/freetype.h"
#include "utils/exception.h"
#include "utils/math.h"
#include <cmath>

namespace gfx {

Font::Font(GlyphCode first_glyph, int nr_glyphs,
           utils::Span<const std::byte> file, int size_px)
{
    // load font
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

    // render glyphs
    struct GlyphBitmap {
        std::vector<uint32_t> pixels;
        utils::Vec2i size;
        utils::Vec2i bearing;
        int advance;
    };
    std::vector<GlyphBitmap> glyphs;
    for (int i = 0; i < nr_glyphs; ++i) {
        GlyphCode code = first_glyph + i;
        if (FT_Error error = FT_Load_Glyph(face, FT_Get_Char_Index(face, code), FT_LOAD_RENDER))
            throw MAKE_EXCEPTION("could not load set face size: error {:#x}", error);

        GlyphBitmap& bitmap = glyphs.emplace_back();
        bitmap.size = utils::Vec2u{face->glyph->bitmap.width, face->glyph->bitmap.rows};
        bitmap.bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top};
        bitmap.advance = face->glyph->advance.x;
        bitmap.pixels.resize(bitmap.size.x * bitmap.size.y);
        for (int y = 0; y < bitmap.size.y; ++y)
            for (int x = 0; x < bitmap.size.x; ++x)
                bitmap.pixels[y * bitmap.size.x + x] = (face->glyph->bitmap.buffer[y * face->glyph->bitmap.pitch + x] << 24)
                                                       | 0x00ffffff;
    }

    // pack glyphs to one texture
    std::sort(glyphs.begin(), glyphs.end(), [](const GlyphBitmap& a, const GlyphBitmap& b) {
        return a.size.y > b.size.y;
    });
    static constexpr int texture_width = 512;
    std::vector<uint32_t> texture_pixels;

    utils::Vec2i next_offset{1, 1};
    int next_line_y = 0;
    for (const GlyphBitmap& bitmap : glyphs) {
        utils::Vec2i offset = next_offset;
        if (offset.x + bitmap.size.x >= texture_width)  // go to next line
            offset = {0, next_line_y};

        // copy bitmap into texture
        if (texture_pixels.size() / texture_width < offset.y + bitmap.size.y)  // texture too small
            texture_pixels.resize(utils::ceil2(offset.y + bitmap.size.y) * texture_width);
        for (int y = 0; y < bitmap.size.y; ++y) {
            const uint32_t* bitmap_row = bitmap.pixels.data() + y * bitmap.size.x;
            std::copy(bitmap_row, bitmap_row + bitmap.size.x, texture_pixels.begin() + (offset.y + y) * texture_width + offset.x);
        }

        next_line_y = std::max(next_line_y, offset.y + bitmap.size.y + 1);
        next_offset = {offset.x + bitmap.size.x + 1, offset.y};
    }

    texture_.resize({texture_width, static_cast<int>(texture_pixels.size()) / texture_width}, GL_RGBA8);
    texture_.update(texture_pixels.data(), GL_BGRA);
}

const Texture& Font::texture() const
{
    return texture_;
}

}  // namespace gfx

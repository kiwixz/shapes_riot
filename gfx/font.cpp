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

    FT_Face face;
    if (FT_Error error = FT_New_Memory_Face(freetype,
                                            reinterpret_cast<const FT_Byte*>(file.data()),
                                            static_cast<FT_Long>(file.size()),
                                            0, &face))
        throw MAKE_EXCEPTION("could not load font face: error {:#x}", error);

    if (FT_Error error = FT_Set_Pixel_Sizes(face, 0, size_px))
        throw MAKE_EXCEPTION("could not load set face size: error {:#x}", error);

    utils::Vec2i glyph_size{static_cast<int>(std::ceil((face->bbox.xMax - face->bbox.xMin) * size_px / face->units_per_EM)),
                            static_cast<int>(std::ceil((face->bbox.yMax - face->bbox.yMin) * size_px / face->units_per_EM))};

    static constexpr int texture_width = 256;
    int nr_glyphs_per_row = static_cast<int>(texture_width / glyph_size.x);
    int min_texture_height = utils::div_ceil(nr_glyphs, nr_glyphs_per_row) * glyph_size.y;
    utils::Vec2i texture_size{texture_width, utils::ceil2(min_texture_height)};
    texture_.resize(texture_size, GL_RGBA8);

    for (int i = 0; i < nr_glyphs; ++i) {
        GlyphCode code = first_glyph + i;
        if (FT_Error error = FT_Load_Glyph(face, FT_Get_Char_Index(face, code), FT_LOAD_RENDER))
            throw MAKE_EXCEPTION("could not load set face size: error {:#x}", error);

        FT_Bitmap& bitmap = face->glyph->bitmap;

        std::vector<uint32_t> pixels(glyph_size.x * glyph_size.y, 0xffff00ff);
        utils::Vec2i offset{0, 0};
        for (int y = 0; y < bitmap.rows; ++y)
            for (int x = 0; x < bitmap.width; ++x)
                pixels[(offset.y + y) * glyph_size.x + (offset.x + x)] = 0x00ffffff | (bitmap.buffer[y * bitmap.pitch + x] << 24);

        texture_.update(pixels.data(),
                        utils::Vec2i{i % nr_glyphs_per_row, i / nr_glyphs_per_row} * glyph_size,
                        glyph_size, GL_BGRA);
    }
}

const Texture& Font::texture() const
{
    return texture_;
}

}  // namespace gfx

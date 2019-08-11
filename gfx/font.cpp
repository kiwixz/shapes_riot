#include "gfx/font.h"
#include "gfx/freetype.h"
#include "utils/exception.h"

namespace gfx {

Font::Font(GlyphCode first_glyph, int nr_glyph,
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

    GlyphCode code_end = first_glyph + nr_glyph;
    for (GlyphCode code = first_glyph; code < code_end; ++code) {
        if (FT_Error error = FT_Load_Glyph(face, FT_Get_Char_Index(face, code), FT_LOAD_RENDER))
            throw MAKE_EXCEPTION("could not load set face size: error {:#x}", error);

        fmt::print("{}\n", face->glyph->bitmap.pitch);
    }
}

utils::ScopeExit Font::bind() const
{
    return texture_.bind();
}

}  // namespace gfx

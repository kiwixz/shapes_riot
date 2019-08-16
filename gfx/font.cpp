#include "gfx/font.h"
#include "gfx/freetype.h"
#include "utils/exception.h"
#include "utils/math.h"
#include <cmath>
#include <cstddef>
#include <vector>

namespace gfx {

Font::Font(GlyphCode first_glyph, int nr_glyphs,
           utils::Span<const std::byte> file, int size_px) :
    size_px_{size_px}
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

    if (FT_Error error = FT_Set_Pixel_Sizes(face, 0, size_px_))
        throw MAKE_EXCEPTION("could not load set face size: error {:#x}", error);

    // render glyphs
    struct GlyphBitmap {
        GlyphCode code;
        Glyph glyph;
        std::vector<uint32_t> pixels;
    };
    std::vector<GlyphBitmap> glyphs;
    for (int i = 0; i < nr_glyphs; ++i) {
        GlyphBitmap& bitmap = glyphs.emplace_back();
        bitmap.code = first_glyph + i;
        if (FT_Error error = FT_Load_Glyph(face, FT_Get_Char_Index(face, bitmap.code), FT_LOAD_RENDER))
            throw MAKE_EXCEPTION("could not load set face size: error {:#x}", error);

        bitmap.glyph.size = utils::Vec2u{face->glyph->bitmap.width, face->glyph->bitmap.rows};
        bitmap.glyph.bearing = {face->glyph->bitmap_left, face->glyph->bitmap_top};
        bitmap.glyph.advance = static_cast<int>(face->glyph->advance.x / 64);  // get font units (FT_LOAD_LINEAR_DESIGN is not working)

        bitmap.pixels.resize(bitmap.glyph.size.x * bitmap.glyph.size.y);
        for (int y = 0; y < bitmap.glyph.size.y; ++y)
            for (int x = 0; x < bitmap.glyph.size.x; ++x)
                bitmap.pixels[y * bitmap.glyph.size.x + x] = (face->glyph->bitmap.buffer[y * face->glyph->bitmap.pitch + x] << 24)
                                                             | 0x00ffffff;
    }

    // pack glyphs to one texture
    std::sort(glyphs.begin(), glyphs.end(), [](const GlyphBitmap& a, const GlyphBitmap& b) {
        return a.glyph.size.y > b.glyph.size.y;
    });
    static constexpr int texture_width = 512;
    std::vector<uint32_t> texture_pixels;

    utils::Vec2i next_offset{1, 1};
    int next_line_y = 0;
    for (GlyphBitmap& bitmap : glyphs) {
        Glyph& glyph = glyphs_[bitmap.code];
        glyph = std::move(bitmap.glyph);

        utils::Vec2i offset = next_offset;
        if (offset.x + glyph.size.x >= texture_width)  // go to next line
            offset = {0, next_line_y};

        glyph.uv_offset = offset;  // will be ajusted later

        // copy bitmap into texture
        int texture_height = static_cast<int>(texture_pixels.size() / texture_width);
        if (texture_height < offset.y + glyph.size.y)  // texture too small
            texture_pixels.resize(utils::ceil2(offset.y + glyph.size.y) * texture_width);
        for (int y = 0; y < glyph.size.y; ++y) {
            const uint32_t* bitmap_row = bitmap.pixels.data() + y * glyph.size.x;
            std::copy(bitmap_row, bitmap_row + glyph.size.x, texture_pixels.begin() + (offset.y + y) * texture_width + offset.x);
        }

        next_line_y = std::max(next_line_y, offset.y + glyph.size.y + 1);
        next_offset = {offset.x + glyph.size.x + 1, offset.y};
    }

    utils::Vec2i texture_size{texture_width, static_cast<int>(texture_pixels.size() / texture_width)};

    for (std::pair<const GlyphCode, Glyph>& pair : glyphs_) {
        Glyph& glyph = pair.second;
        glyph.uv_offset /= utils::Vec2f{texture_size};
        glyph.uv_size = utils::Vec2f{glyph.size} / utils::Vec2f{texture_size};
    }

    texture_.resize(texture_size, GL_RGBA8);
    texture_.update(texture_pixels.data(), GL_BGRA);
}

DrawList Font::draw_text(std::string_view text, utils::Vec2f pen, float height) const
{
    DrawList draw_list;
    for (char c : text) {
        const Glyph& glyph = glyphs_.at(static_cast<unsigned char>(c));
        if (glyph.size != utils::Vec2i{}) {
            float aspect_ratio = static_cast<float>(glyph.size.x) / glyph.size.y;
            utils::Vec2f bottom_left = pen + utils::Vec2f{glyph.bearing} / static_cast<float>(size_px_) * height;
            utils::Vec2f size{height * aspect_ratio, height};
            draw_list.push_quad({bottom_left, {glyph.uv_offset.x, glyph.uv_offset.y + glyph.uv_size.y}},
                                {{bottom_left.x, bottom_left.y + size.y}, glyph.uv_offset},
                                {bottom_left + size, {glyph.uv_offset.x + glyph.uv_size.x, glyph.uv_offset.y}},
                                {{bottom_left.x + size.x, bottom_left.y}, glyph.uv_offset + glyph.uv_size},
                                &texture_);
        }
        pen.x += static_cast<float>(glyph.advance) / size_px_ * height;
    }
    return draw_list;
}

const Texture& Font::texture() const
{
    return texture_;
}

}  // namespace gfx

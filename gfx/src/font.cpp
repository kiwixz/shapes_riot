#include "gfx/font.h"
#include <cmath>
#include <cstddef>
#include <vector>
#include "gfx/freetype.h"
#include "gfx/transformation.h"
#include "utils/exception.h"
#include "utils/math.h"

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
        GlyphCode code;
        Glyph glyph;
        std::vector<uint32_t> pixels;
        utils::Vec2i size;
    };
    std::vector<GlyphBitmap> glyphs;
    for (int i = 0; i < nr_glyphs; ++i) {
        GlyphBitmap& bitmap = glyphs.emplace_back();
        bitmap.code = first_glyph + i;
        if (FT_Error error = FT_Load_Glyph(face, FT_Get_Char_Index(face, bitmap.code), FT_LOAD_RENDER))
            throw MAKE_EXCEPTION("could not load set face size: error {:#x}", error);

        bitmap.size = utils::Vec2u{face->glyph->bitmap.width, face->glyph->bitmap.rows};
        bitmap.glyph.size = utils::Vec2f{bitmap.size} / static_cast<float>(size_px);
        bitmap.glyph.bearing = utils::Vec2f{utils::Vec2i{face->glyph->bitmap_left, face->glyph->bitmap_top}}
                               / static_cast<float>(size_px);
        bitmap.glyph.advance = static_cast<float>(face->glyph->advance.x) / 64  // convert to font units (FT_LOAD_LINEAR_DESIGN is not working)
                               / size_px;

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
    static constexpr int texture_width = 8192;
    std::vector<uint32_t> texture_pixels;

    utils::Vec2i next_offset{1, 1};
    int next_line_y = 0;
    for (GlyphBitmap& bitmap : glyphs) {
        Glyph& glyph = glyphs_[bitmap.code];
        glyph = std::move(bitmap.glyph);

        utils::Vec2i offset = next_offset;
        if (offset.x + bitmap.size.x >= texture_width)  // go to next line
            offset = {0, next_line_y};

        glyph.uv_offset = offset;
        glyph.uv_size = bitmap.size;

        // copy bitmap into texture
        int texture_height = static_cast<int>(texture_pixels.size() / texture_width);
        if (texture_height < offset.y + bitmap.size.y)  // texture too small
            texture_pixels.resize(utils::ceil2(offset.y + bitmap.size.y) * texture_width);
        for (int y = 0; y < bitmap.size.y; ++y) {
            const uint32_t* bitmap_row = bitmap.pixels.data() + y * bitmap.size.x;
            std::copy(bitmap_row, bitmap_row + bitmap.size.x, texture_pixels.begin() + (offset.y + y) * texture_width + offset.x);
        }

        next_line_y = std::max(next_line_y, offset.y + bitmap.size.y + 1);
        next_offset = {offset.x + bitmap.size.x + 1, offset.y};
    }

    utils::Vec2i texture_size{texture_width, static_cast<int>(texture_pixels.size() / texture_width)};

    for (std::pair<const GlyphCode, Glyph>& pair : glyphs_) {
        Glyph& glyph = pair.second;
        glyph.uv_offset /= utils::Vec2f{texture_size};
        glyph.uv_size /= utils::Vec2f{texture_size};
    }

    texture_.resize(texture_size, GL_RGBA8);
    texture_.update(texture_pixels.data(), GL_BGRA);
}

DrawList Font::draw_glyph(GlyphCode code, utils::Vec3f pen, utils::Vec2f size) const
{
    return draw_glyph(glyphs_.at(code), pen, size);
}

DrawList Font::draw_text(std::string_view text, utils::Vec3f center, utils::Vec2f size) const
{
    DrawList draw_list;
    utils::Vec3f pen;
    float y_min = 0.0f;
    float y_max = 0.0f;
    for (char c : text) {
        const Glyph& glyph = glyphs_.at(static_cast<unsigned char>(c));
        draw_list += draw_glyph(glyph, pen, size);
        pen.x += glyph.advance * size.x;
        y_min = std::min(y_min, glyph.bearing.y - glyph.size.y);
        y_max = std::max(y_max, glyph.bearing.y);
    }
    draw_list.transform(translation(utils::Vec3f{center.x - pen.x / 2,
                                                 center.y - (y_max + y_min) / 2,
                                                 0.0f}));
    return draw_list;
}

DrawList Font::draw_text_linear(std::string_view text, utils::Vec3f& pen, utils::Vec2f size) const
{
    DrawList draw_list;
    for (char c : text) {
        const Glyph& glyph = glyphs_.at(static_cast<unsigned char>(c));
        draw_list += draw_glyph(glyph, pen, size);
        pen.x += glyph.advance * size.x;
    }
    return draw_list;
}

const Texture& Font::texture() const
{
    return texture_;
}

DrawList Font::draw_glyph(const Glyph& glyph, utils::Vec3f pen, utils::Vec2f size) const
{
    DrawList draw_list;
    if (glyph.size != utils::Vec2f{}) {
        utils::Vec3f bottom_left = pen + utils::Vec3f{glyph.bearing.x * size.x, (glyph.bearing.y - glyph.size.y) * size.y, 0.0f};
        utils::Vec3f quad_size{glyph.size * size, 0.0f};
        draw_list.push_quad({bottom_left, {glyph.uv_offset.x, glyph.uv_offset.y + glyph.uv_size.y}},
                            {{bottom_left.x, bottom_left.y + quad_size.y, bottom_left.z}, glyph.uv_offset},
                            {bottom_left + quad_size, {glyph.uv_offset.x + glyph.uv_size.x, glyph.uv_offset.y}},
                            {{bottom_left.x + quad_size.x, bottom_left.y, bottom_left.z}, glyph.uv_offset + glyph.uv_size},
                            texture_);
    }
    return draw_list;
}

}  // namespace gfx

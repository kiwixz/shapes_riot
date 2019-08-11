#include "gfx/font.h"

namespace gfx {

Font::Font(utils::Span<const std::byte> file)
{}

utils::ScopeExit Font::bind() const
{
    return texture_.bind();
}

}  // namespace gfx

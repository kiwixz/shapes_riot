#include "gfx/texture.h"

namespace gfx {

Texture::Texture(utils::Vec2i size, GLenum format) :
    size_{size}
{
    glCreateTextures(GL_TEXTURE_2D, 1, texture_.ptr());
    glTextureStorage2D(texture_[0], 1, format, size_.x, size_.y);
}

utils::ScopeExit Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, texture_[0]);
    return utils::ScopeExit{std::bind(glBindTexture, GL_TEXTURE_2D, 0)};
}

}  // namespace gfx

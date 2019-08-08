#include "gfx/texture.h"

namespace gfx {

Texture::Texture(int width, int height, GLenum format)
{
    glCreateTextures(GL_TEXTURE_2D, 1, texture_.ptr());
    glTextureStorage2D(texture_[0], 1, format, width, height);
}

utils::ScopeExit Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, texture_[0]);
    return utils::ScopeExit{std::bind(glBindTexture, GL_TEXTURE_2D, 0)};
}

}  // namespace gfx

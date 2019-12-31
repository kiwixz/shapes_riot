#include "gfx/texture.h"

namespace gfx {

Texture::Texture(utils::Vec2i size, int levels, GLenum format)
{
    resize(size, levels, format);
}

Texture::operator TextureView() const
{
    return TextureView{texture_[0]};
}

utils::Vec2i Texture::size() const
{
    return size_;
}

utils::ScopeExit Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, texture_[0]);
    return utils::ScopeExit{std::bind(glBindTexture, GL_TEXTURE_2D, 0)};
}

void Texture::resize(utils::Vec2i size, int levels, GLenum format)
{
    if (texture_[0])  // already allocated
        texture_ = {};
    size_ = size;
    glCreateTextures(GL_TEXTURE_2D, 1, texture_.ptr());
    glTextureStorage2D(texture_[0], levels, format, size_.x, size_.y);
}

void Texture::generate_mipmaps()
{
    glGenerateTextureMipmap(texture_[0]);
}

}  // namespace gfx

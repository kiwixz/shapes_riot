#pragma once

#include "gfx/gl_object.h"
#include "utils/scope_exit.h"
#include "utils/vec.h"
#include <glad/glad.h>

namespace gfx {

struct Texture {
    Texture() = default;
    Texture(utils::Vec2i size, GLenum format = GL_RGB8);

    utils::Vec2i size() const;
    [[nodiscard]] utils::ScopeExit bind() const;

    void resize(utils::Vec2i, GLenum format = GL_RGB8);

    template <typename T>
    void upload(const T* pixels, GLenum format = GL_BGR, GLenum type = GL_UNSIGNED_BYTE);

private:
    utils::Vec2i size_;
    GlTextures<> texture_;
};


template <typename T>
void Texture::upload(const T* pixels, GLenum format, GLenum type)
{
    glTextureSubImage2D(texture_[0], 0, 0, 0, size_.x, size_.y, format, type, pixels);
}

}  // namespace gfx

#pragma once

#include "gfx/gl_object.h"
#include "utils/scope_exit.h"
#include <glad/glad.h>

namespace gfx {

struct Texture {
    Texture(int width, int height, GLenum format = GL_RGB8);

    [[nodiscard]] utils::ScopeExit bind() const;

    template <typename T>
    void upload(const T* pixels, GLenum format = GL_BGR, GLenum type = GL_UNSIGNED_BYTE);

private:
    GlTextures<> texture_;
    int width_;
    int height_;
};


template <typename T>
void Texture::upload(const T* pixels, GLenum format, GLenum type)
{
    glTextureSubImage2D(texture_[0], 0, 0, 0, width_, height_, format, type, pixels);
}

}  // namespace gfx

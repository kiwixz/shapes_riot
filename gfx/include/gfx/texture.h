#pragma once

#include <glad/glad.h>

#include "gfx/gl_object.h"
#include "gfx/texture_view.h"
#include "utils/scope_exit.h"
#include "utils/vec.h"

namespace gfx {

struct Texture {
    Texture() = default;
    Texture(utils::Vec2i size, int levels = 1, GLenum format = GL_RGB8);

    operator TextureView() const;

    utils::Vec2i size() const;

    [[nodiscard]] utils::ScopeExit bind() const;
    void resize(utils::Vec2i size, int levels = 1, GLenum format = GL_RGB8);
    void generate_mipmaps();

    template <typename T>
    void update(const T* pixels, utils::Vec2i offset, utils::Vec2i size, int level = 0,
                GLenum format = GL_BGR, GLenum type = GL_UNSIGNED_BYTE)
    {
        glTextureSubImage2D(texture_[0], level, offset.x, offset.y, size.x, size.y, format, type, pixels);
    }

    template <typename T>
    void update(const T* pixels, int level = 0, GLenum format = GL_BGR, GLenum type = GL_UNSIGNED_BYTE)
    {
        glTextureSubImage2D(texture_[0], level, 0, 0, size_.x, size_.y, format, type, pixels);
    }

private:
    utils::Vec2i size_;
    GlTextures<> texture_;
};

}  // namespace gfx

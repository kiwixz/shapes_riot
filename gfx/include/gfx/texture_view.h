#pragma once

#include "utils/scope_exit.h"
#include <glad/glad.h>

namespace gfx {

struct TextureView {
    TextureView() = default;
    explicit TextureView(GLuint id);

    explicit operator bool() const;

    [[nodiscard]] utils::ScopeExit bind() const;

private:
    GLuint id_ = 0;

    friend bool operator==(TextureView lhs, TextureView rhs);
};


bool operator==(TextureView lhs, TextureView rhs);
bool operator!=(TextureView lhs, TextureView rhs);

}  // namespace gfx

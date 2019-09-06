#pragma once

#include "utils/scope_exit.h"
#include <glad/glad.h>

namespace gfx {

struct TextureView {
    explicit TextureView(GLuint id);

    [[nodiscard]] utils::ScopeExit bind() const;

private:
    GLuint id_;
};

}  // namespace gfx

#pragma once

#include "gfx/gl_object.h"
#include "utils/scope_exit.h"
#include <glad/glad.h>
#include <string_view>

namespace gfx {

struct ShaderProgram;


struct Shader {
    Shader(GLenum type, std::string_view source);

private:
    GlShader shader_;

    friend ShaderProgram;
};


struct ShaderProgram {
public:
    ShaderProgram();

    void attach(const Shader& shader);
    void link();

    [[nodiscard]] utils::ScopeExit bind() const;

private:
    GlProgram program_;
};

}  // namespace gfx

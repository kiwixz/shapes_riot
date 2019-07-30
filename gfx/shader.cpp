#include "gfx/shader.h"
#include "utils/exception.h"

namespace gfx {

Shader::Shader(GLenum type, std::string_view source)
{
    shader_ = GlShader{glCreateShader(type)};
    GLchar const* source_ptr = source.data();
    auto source_length = static_cast<GLint>(source.length());
    glShaderSource(shader_.id(), 1, &source_ptr, &source_length);
    glCompileShader(shader_.id());

    int status;
    glGetShaderiv(shader_.id(), GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader_.id(), GL_INFO_LOG_LENGTH, &length);
        std::vector<char> info(length);
        glGetShaderInfoLog(shader_.id(), length, &length, info.data());
        throw utils::Exception{"could not compile shader: {}", info.data()};
    }
}


ShaderProgram::ShaderProgram()
{
    program_ = GlProgram{glCreateProgram()};
}

void ShaderProgram::attach(Shader const& shader)
{
    glAttachShader(program_.id(), shader.shader_.id());
}

void ShaderProgram::link()
{
    glLinkProgram(program_.id());
}

utils::ScopeExit ShaderProgram::bind()
{
    glUseProgram(program_.id());
    return utils::ScopeExit{&ShaderProgram::unbind};
}

void ShaderProgram::unbind()
{
    glUseProgram(0);
}

}  // namespace gfx

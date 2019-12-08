#include "gfx/drawer.h"

#include <string_view>

#include <glad/glad.h>

namespace gfx {

Drawer::Drawer(utils::ResourceManager& resource_manager)
{
    resource_manager.get_to("gfx_drawer_program", program_, [] {
        constexpr std::string_view vertex_source = R"(
            #version 450
            layout(location=0) in vec4 pos;
            layout(location=1) in vec4 rgb;
            layout(location=2) in vec2 uv;
            layout(location=0) out vec4 rgb_;
            layout(location=1) out vec2 uv_;
            void main() {
                gl_Position = pos;
                rgb_ = rgb;
                uv_ = uv;
            }
        )";
        constexpr std::string_view fragment_source = R"(
            #version 450
            uniform sampler2D tex;
            layout(location=0) in vec4 rgb_;
            layout(location=1) in vec2 uv_;
            out vec4 color;
            void main() {
                color = rgb_ * texture(tex, uv_);
            }
        )";

        auto program = std::make_shared<ShaderProgram>();
        program->attach({GL_VERTEX_SHADER, vertex_source});
        program->attach({GL_FRAGMENT_SHADER, fragment_source});
        program->link();
        return program;
    });

    resource_manager.get_to("gfx_drawer_blank", blank_, [] {
        auto texture = std::make_shared<Texture>(utils::Vec2i{1, 1});
        static constexpr uint32_t blank_pixel = 0xffffffff;
        texture->update(&blank_pixel);
        return texture;
    });

    glCreateVertexArrays(vertex_array_.size, vertex_array_.ptr());
    glCreateBuffers(buffers_.size, buffers_.ptr());

    auto vao_attrib = [&](unsigned attrib, size_t offset, int size) {
        glEnableVertexArrayAttrib(vertex_array_[0], attrib);
        glVertexArrayVertexBuffer(vertex_array_[0], attrib, buffers_[0], offset, sizeof(Vertex));
        glVertexArrayAttribFormat(vertex_array_[0], attrib, size / sizeof(float), GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vertex_array_[0], attrib, attrib);
    };

    vao_attrib(0, offsetof(Vertex, pos), sizeof(Vertex::pos));
    vao_attrib(1, offsetof(Vertex, rgb), sizeof(Vertex::rgb));
    vao_attrib(2, offsetof(Vertex, uv), sizeof(Vertex::uv));

    glVertexArrayElementBuffer(vertex_array_[0], buffers_[1]);
}

void Drawer::draw(const DrawList& draw_list)
{
    utils::ScopeExit program_binding = program_->bind();
    glBindVertexArray(vertex_array_[0]);
    for (const SubDrawList& sub_list : draw_list) {
        utils::ScopeExit texture_binding;
        if (sub_list.texture())
            texture_binding = sub_list.texture().bind();
        else
            texture_binding = blank_->bind();

        glNamedBufferData(buffers_[0], sub_list.vertices().size_bytes(), sub_list.vertices().data(), GL_STREAM_DRAW);
        glNamedBufferData(buffers_[1], sub_list.indexes().size_bytes(), sub_list.indexes().data(), GL_STREAM_DRAW);
        glDrawElements(GL_TRIANGLES, static_cast<int>(sub_list.indexes().size()), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}

}  // namespace gfx

#include "gfx/drawer.h"
#include <glad/glad.h>
#include <string_view>

namespace gfx {

Drawer::Drawer()
{
    constexpr std::string_view vertex_source = R"(
            #version 450
            layout(location=0) in vec2 pos;
            layout(location=1) in vec4 color;
            layout(location=0) out vec4 color_v;
            void main() {
                gl_Position = vec4(pos, 0.0, 1.0);
                color_v = color;
            }
        )";
    constexpr std::string_view fragment_source = R"(
            #version 450
            layout(location=0) in vec4 color_v;
            out vec4 color;
            void main() {
                color = color_v;
            }
        )";
    program_.attach({GL_VERTEX_SHADER, vertex_source});
    program_.attach({GL_FRAGMENT_SHADER, fragment_source});
    program_.link();

    glCreateVertexArrays(vertex_array_.size, vertex_array_.ptr());
    glCreateBuffers(buffers_.size, buffers_.ptr());

    int pos_attrib = 0;
    glEnableVertexArrayAttrib(vertex_array_[0], pos_attrib);
    glVertexArrayVertexBuffer(vertex_array_[0], 0, buffers_[0],
                              offsetof(DrawList::ColoredVertex, pos), sizeof(DrawList::ColoredVertex));
    glVertexArrayAttribFormat(vertex_array_[0], pos_attrib,
                              sizeof(DrawList::ColoredVertex::pos) / sizeof(float), GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vertex_array_[0], pos_attrib, 0);

    int color_attrib = 1;
    glEnableVertexArrayAttrib(vertex_array_[0], color_attrib);
    glVertexArrayVertexBuffer(vertex_array_[0], 1, buffers_[0],
                              offsetof(DrawList::ColoredVertex, color), sizeof(DrawList::ColoredVertex));
    glVertexArrayAttribFormat(vertex_array_[0], color_attrib,
                              sizeof(DrawList::ColoredVertex::color) / sizeof(float), GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vertex_array_[0], color_attrib, 1);

    glVertexArrayElementBuffer(vertex_array_[0], buffers_[1]);
}

void Drawer::draw(const DrawList& draw_list)
{
    glBindVertexArray(vertex_array_[0]);
    {
        utils::ScopeExit program_binding = program_.bind();
        glNamedBufferData(buffers_[0], draw_list.vertices().size_bytes(), draw_list.vertices().data(), GL_STREAM_DRAW);
        glNamedBufferData(buffers_[1], draw_list.indexes().size_bytes(), draw_list.indexes().data(), GL_STREAM_DRAW);
        glDrawElements(GL_TRIANGLES, static_cast<int>(draw_list.indexes().size()), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}

}  // namespace gfx

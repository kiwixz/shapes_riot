#include "gfx/draw_list.h"
#include <glad/glad.h>
#include <algorithm>
#include <string_view>

namespace gfx {

DrawList::DrawList()
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
    glVertexArrayVertexBuffer(vertex_array_[0], 0, buffers_[0], offsetof(ColoredVertex, pos), sizeof(ColoredVertex));
    glVertexArrayAttribFormat(vertex_array_[0], pos_attrib, sizeof(ColoredVertex::pos) / sizeof(float), GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vertex_array_[0], pos_attrib, 0);

    int color_attrib = 1;
    glEnableVertexArrayAttrib(vertex_array_[0], color_attrib);
    glVertexArrayVertexBuffer(vertex_array_[0], 1, buffers_[0], offsetof(ColoredVertex, color), sizeof(ColoredVertex));
    glVertexArrayAttribFormat(vertex_array_[0], color_attrib, sizeof(ColoredVertex::color) / sizeof(float), GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vertex_array_[0], color_attrib, 1);

    glVertexArrayElementBuffer(vertex_array_[0], buffers_[1]);

    clear();
}

void DrawList::clear()
{
    vertices_.clear();
    next_index_ = 0;
}

void DrawList::push(const std::vector<ColoredVertex>& vertices)
{
    vertices_.reserve(vertices_.size() + vertices.size());
    std::copy(vertices.begin(), vertices.end(), std::back_inserter(vertices_));

    indexes_.reserve(indexes_.size() + vertices.size());
    for (int i = 0; i < static_cast<int>(vertices.size()); ++i)
        indexes_.push_back(next_index_ + i);
    next_index_ += static_cast<int>(vertices.size());
}

void DrawList::push(const std::vector<ColoredVertex>& vertices, const std::vector<Index>& index)
{
    vertices_.reserve(vertices_.size() + vertices.size());
    std::copy(vertices.begin(), vertices.end(), std::back_inserter(vertices_));

    indexes_.reserve(indexes_.size() + index.size());
    for (const auto& idx : index) {
        indexes_.push_back(next_index_ + idx);
        next_index_ = std::max(next_index_, idx + 1);
    }
}

void DrawList::draw()
{
    glBindVertexArray(vertex_array_[0]);
    {
        utils::ScopeExit program_binding = program_.bind();
        glNamedBufferData(buffers_[0], vertices_.size() * sizeof(ColoredVertex), vertices_.data(), GL_STREAM_DRAW);
        glNamedBufferData(buffers_[1], indexes_.size() * sizeof(Index), indexes_.data(), GL_STREAM_DRAW);
        glDrawElements(GL_TRIANGLES, static_cast<int>(indexes_.size()), GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}

}  // namespace gfx

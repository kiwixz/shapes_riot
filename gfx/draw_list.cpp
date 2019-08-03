#include "gfx/draw_list.h"
#include <glad/glad.h>

namespace gfx {

DrawList::DrawList()
{
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
    max_index_ = -1;
}

void DrawList::push(const std::vector<ColoredVertex>& vertices)
{
    vertices_.reserve(vertices_.size() + vertices.size());
    std::copy(vertices.begin(), vertices.end(), std::back_inserter(vertices_));

    indexes_.reserve(indexes_.size() + vertices.size());
    for (int i = 0; i < static_cast<int>(vertices.size()); ++i)
        indexes_.push_back(max_index_ + 1 + i);
    max_index_ += static_cast<int>(vertices.size());
}

void DrawList::push(const std::vector<ColoredVertex>& vertices, const std::vector<Index>& index)
{
    vertices_.reserve(vertices_.size() + vertices.size());
    std::copy(vertices.begin(), vertices.end(), std::back_inserter(vertices_));

    indexes_.reserve(indexes_.size() + index.size());
    for (const auto& idx : index) {
        indexes_.push_back(max_index_ + 1 + idx);
        max_index_ = std::max(max_index_, idx);
    }
}

void DrawList::draw()
{
    glBindVertexArray(vertex_array_[0]);
    glNamedBufferData(buffers_[0], vertices_.size() * sizeof(ColoredVertex), vertices_.data(), GL_STREAM_DRAW);
    glNamedBufferData(buffers_[1], indexes_.size() * sizeof(Index), indexes_.data(), GL_STREAM_DRAW);
    glDrawElements(GL_TRIANGLES, static_cast<int>(indexes_.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

}  // namespace gfx

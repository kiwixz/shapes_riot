#pragma once

#include "gfx/draw_list.h"
#include "gfx/gl_object.h"
#include "gfx/shader.h"
#include "utils/resource_manager.h"

namespace gfx {

struct Drawer {
    explicit Drawer(utils::ResourceManager& resource_manager);

    void draw(const DrawList& draw_list);

private:
    utils::ResourceHandle<ShaderProgram> program_;
    GlBuffers<2> buffers_;
    GlVertexArrays<> vertex_array_;
    Texture blank_;
};

}  // namespace gfx

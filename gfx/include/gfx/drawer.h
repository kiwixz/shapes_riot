#pragma once

#include "gfx/draw_list.h"
#include "gfx/gl_object.h"
#include "gfx/shader.h"

namespace gfx {

struct Drawer {
    Drawer();

    void draw(const DrawList& draw_list);

private:
    ShaderProgram program_;
    GlBuffers<2> buffers_;
    GlVertexArrays<> vertex_array_;
};

}  // namespace gfx

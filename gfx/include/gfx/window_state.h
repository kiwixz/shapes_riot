#pragma once

#include "glad_glfw.h"
#include "utils/vec.h"

namespace gfx {

struct WindowState {
    explicit WindowState(GLFWwindow* window);

    bool iconified;
    utils::Vec2i window_size;
    utils::Vec2i framebuffer_size;
    utils::Vec2d mouse_pos;
};

}  // namespace gfx

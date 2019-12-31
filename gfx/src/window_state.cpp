#include "gfx/window_state.h"

namespace gfx {

WindowState::WindowState(GLFWwindow& window)
{
    iconified = glfwGetWindowAttrib(&window, GLFW_ICONIFIED);
    glfwGetWindowSize(&window, &window_size.x, &window_size.y);
    glfwGetFramebufferSize(&window, &framebuffer_size.x, &framebuffer_size.y);

    utils::Vec2d mouse_pos_screen;
    glfwGetCursorPos(&window, &mouse_pos_screen.x, &mouse_pos_screen.y);
    mouse_pos = utils::Vec2d{mouse_pos_screen.x / window_size.x * 2.0 - 1.0,
                             -(mouse_pos_screen.y / window_size.y * 2.0 - 1.0)};
}

}  // namespace gfx

#include "gfx/window.h"
#include "utils/exception.h"

namespace gfx {

Window::Window(utils::Vec2i size, std::string_view title, GLFWmonitor* monitor, Window* shared)
{
    GLFWwindow* window = glfwCreateWindow(size.x, size.y, title.data(), monitor, shared ? shared->window_.get() : nullptr);
    if (!window)
        throw utils::Exception{"could not create window"};
    window_.reset(window);

    bind_context();

    if (!gladLoadGLLoader([](char const* name) {
            return reinterpret_cast<void*>(glfwGetProcAddress(name));
        }))
        throw utils::Exception{"could not load opengl"};

    glfwSwapInterval(1);
}

void Window::bind_context()
{
    glfwMakeContextCurrent(window_.get());
}

}  // namespace gfx

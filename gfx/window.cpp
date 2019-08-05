#include "gfx/window.h"
#include "utils/exception.h"

namespace gfx {

Window::Window(utils::Vec2i size, std::string_view title, GLFWmonitor* monitor, Window* shared)
{
    GLFWwindow* window = glfwCreateWindow(size.x, size.y, title.data(), monitor, shared ? shared->window_.get() : nullptr);
    if (!window)
        throw MAKE_EXCEPTION("could not create window");
    window_.reset(window);

    glfwMakeContextCurrent(window_.get());

    if (!gladLoadGLLoader([](char const* name) {
            return reinterpret_cast<void*>(glfwGetProcAddress(name));
        }))
        throw MAKE_EXCEPTION("could not load opengl");
}

GLFWwindow* Window::ptr()
{
    return window_.get();
}

GLFWwindow const* Window::ptr() const
{
    return window_.get();
}

}  // namespace gfx

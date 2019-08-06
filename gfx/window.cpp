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

    glfwSetWindowUserPointer(window_.get(), this);
    glfwSetKeyCallback(window_.get(), [](GLFWwindow* glfw_window, int key, int scancode, int action, int mods) {
        return reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfw_window))->on_key(key, scancode, action, mods);
    });
}

void Window::poll_events(const std::function<void(WindowEvent&&)>& event_handler)
{
    glfwPollEvents();
    for (WindowEvent& event : event_queue_)
        event_handler(std::move(event));
    event_queue_.clear();
}

GLFWwindow* Window::ptr()
{
    return window_.get();
}

GLFWwindow const* Window::ptr() const
{
    return window_.get();
}

void Window::on_key(int key, int scancode, int action, int mods)
{
    event_queue_.emplace_back(WindowEvent::KeyEvent{key, scancode, action, mods});
}

}  // namespace gfx

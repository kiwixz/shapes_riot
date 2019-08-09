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
    glfwSetFramebufferSizeCallback(window_.get(), [](GLFWwindow* glfw_window, int width, int height) {
        Window& window = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        window.event_queue_.emplace_back(WindowEvent::FramebufferResize{width, height});
    });
    glfwSetKeyCallback(window_.get(), [](GLFWwindow* glfw_window, int key, int scancode, int action, int mods) {
        Window& window = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        window.event_queue_.emplace_back(WindowEvent::KeyEvent{key, scancode, action, mods});
    });
}

WindowState Window::state() const
{
    return WindowState{window_.get()};
}

GLFWwindow* Window::ptr()
{
    return window_.get();
}

GLFWwindow const* Window::ptr() const
{
    return window_.get();
}

void Window::poll_events(const std::function<void(WindowEvent&& event)>& event_handler)
{
    glfwPollEvents();
    for (WindowEvent& event : event_queue_)
        event_handler(std::move(event));
    event_queue_.clear();
}

void Window::on_key(int key, int scancode, int action, int mods)
{
    event_queue_.emplace_back(WindowEvent::KeyEvent{key, scancode, action, mods});
}

}  // namespace gfx

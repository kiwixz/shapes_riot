#include "gfx/window.h"

#include "utils/exception.h"

namespace gfx {

Window::Window(std::string_view title, utils::Vec2i size)
{
    logger_(utils::LogLevel::info, "initializing glfw");
    glfwSetErrorCallback([](int error, const char* description) {
        utils::Logger{"GLFW"}(utils::LogLevel::error, "error {:#x}: {}", error, description);
    });
    glfw_handle_.emplace();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    logger_(utils::LogLevel::info, "creating window");
    if (!utils::set_c_ptr(window_, glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr)))
        throw MAKE_EXCEPTION("could not create window");

    logger_(utils::LogLevel::info, "loading gl");
    glfwMakeContextCurrent(window_.get());
    if (!gladLoadGLLoader([](const char* name) { return reinterpret_cast<void*>(glfwGetProcAddress(name)); }))
        throw MAKE_EXCEPTION("could not load opengl");
    logger_(utils::LogLevel::info, "got gl context with version: {}", glGetString(GL_VERSION));

    logger_(utils::LogLevel::info, "setting up window");
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window_.get(), this);
    glfwSetFramebufferSizeCallback(window_.get(), [](GLFWwindow* glfw_window, int width, int height) {
        Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        self.event_queue_.lock()->emplace_back(WindowEvent::FramebufferResize{width, height});
    });
    glfwSetKeyCallback(window_.get(), [](GLFWwindow* glfw_window, int key, int scancode, int action, int mods) {
        Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        self.event_queue_.lock()->emplace_back(WindowEvent::KeyEvent{key, scancode, action, mods});
    });
    glfwSetMouseButtonCallback(window_.get(), [](GLFWwindow* glfw_window, int button, int action, int mods) {
        Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        self.event_queue_.lock()->emplace_back(WindowEvent::MouseButtonEvent{button, action, mods});
    });
}

bool Window::is_closed() const
{
    return glfwWindowShouldClose(window_.get());
}

WindowState Window::state() const
{
    return WindowState{window_.get()};
}

void Window::swap_buffers()
{
    glfwSwapBuffers(window_.get());
}

}  // namespace gfx

#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "gfx/glad_glfw.h"
#include "gfx/glfw_handle.h"
#include "gfx/window_event.h"
#include "gfx/window_state.h"
#include "utils/c_ptr.h"
#include "utils/guarded.h"
#include "utils/logger.h"
#include "utils/vec.h"

namespace gfx {

struct Window {
    Window(std::string_view title, utils::Vec2i size);
    ~Window() = default;
    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;
    Window(Window&& other) noexcept = delete;
    Window& operator=(Window&& other) noexcept = delete;

    bool is_closed() const;
    WindowState state() const;

    void swap_buffers();

    /// Will call event_handler with a WindowEvent&& as argument.
    template <typename T>
    void poll_events(T&& event_handler);

private:
    utils::Logger logger_{"Window"};
    std::optional<GlfwHandle> glfw_handle_;
    utils::CPtr<GLFWwindow, glfwDestroyWindow> window_;
    utils::Guarded<std::vector<WindowEvent>> event_queue_;
};


template <typename T>
void Window::poll_events(T&& event_handler)
{
    glfwPollEvents();
    std::vector<WindowEvent> current_event_queue;
    std::swap(current_event_queue, *event_queue_.lock());
    for (WindowEvent& event : current_event_queue)
        event_handler(std::move(event));
}

}  // namespace gfx

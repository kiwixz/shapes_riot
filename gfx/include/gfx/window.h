#pragma once

#include "gfx/glfw_handle.h"
#include "gfx/window_event.h"
#include "utils/vec.h"
#include <functional>
#include <memory>
#include <string_view>

namespace gfx {

struct Window {
    Window() = default;
    Window(utils::Vec2i size, std::string_view title, GLFWmonitor* monitor = nullptr, Window* shared = nullptr);
    ~Window() = default;
    Window(const Window& other) noexcept = delete;
    Window& operator=(const Window& other) noexcept = delete;
    Window(Window&& other) noexcept = delete;
    Window& operator=(Window&& other) noexcept = delete;

    void poll_events(const std::function<void(WindowEvent&&)>& event_handler);

    [[nodiscard]] GLFWwindow* ptr();
    [[nodiscard]] GLFWwindow const* ptr() const;

private:
    GlfwHandle glfw_handle_;
    std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> window_{nullptr, glfwDestroyWindow};
    std::vector<WindowEvent> event_queue_;

    void on_key(int key, int scancode, int action, int mods);
};

}  // namespace gfx

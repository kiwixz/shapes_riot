#pragma once

#include "gfx/glfw_handle.h"
#include "utils/vec.h"
#include <memory>
#include <string_view>

namespace gfx {

struct Window {
    Window() = default;
    Window(utils::Vec2i size, std::string_view title, GLFWmonitor* monitor = nullptr, Window* shared = nullptr);

    [[nodiscard]] GLFWwindow* ptr();
    [[nodiscard]] GLFWwindow const* ptr() const;

private:
    GlfwHandle glfw_handle_;
    std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> window_{nullptr, glfwDestroyWindow};
};

}  // namespace gfx

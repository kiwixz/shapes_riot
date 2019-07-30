#pragma once

#include "gfx/shader.h"
#include "gfx/window.h"
#include "screen_stack.h"

namespace shapes_riot {

struct MainWindow {
    MainWindow();

    void loop();

private:
    struct Glfw {
        Glfw();

    private:
        gfx::GlfwHandle handle_;
    };

    Glfw glfw_;
    gfx::Window window_;
    ScreenStack screens_;
    gfx::ShaderProgram program_;

    void on_key(int key, int scancode, int action, int mods);
};

}  // namespace shapes_riot

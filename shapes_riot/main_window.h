#pragma once

#include "gfx/window.h"
#include "screen_stack.h"

namespace shapes_riot {

struct MainWindow {
    MainWindow();

    void loop();

private:
    gfx::Window window_;
    ScreenStack screens_;

    void on_key(int key, int scancode, int action, int mods);
};

}  // namespace shapes_riot

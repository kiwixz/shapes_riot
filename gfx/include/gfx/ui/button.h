#pragma once

#include "gfx/ui/widget.h"

namespace gfx::ui {

struct Button : Widget {
    Button();

    DrawList draw(double delta) override;

    void on_key(const WindowEvent::KeyEvent& event) override;
    void on_mouse_button(const WindowEvent::KeyEvent& event, utils::Vec2d pos) override;

private:
};

}  // namespace gfx::ui

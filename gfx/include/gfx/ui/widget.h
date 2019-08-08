#pragma once

#include "gfx/draw_list.h"
#include "gfx/window_event.h"
#include "gfx/window_state.h"

namespace gfx::ui {

struct Widget {
    virtual ~Widget() = default;

    virtual DrawList draw(double delta) = 0;

    virtual void on_key(const WindowEvent::KeyEvent& event) = 0;
    virtual void on_mouse_button(const WindowEvent::KeyEvent& event, utils::Vec2d pos) = 0;

    void on_window_event(const WindowEvent& event, const WindowState& state);
};

}  // namespace gfx::ui

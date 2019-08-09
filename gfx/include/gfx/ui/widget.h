#pragma once

#include "gfx/draw_list.h"
#include "gfx/window_event.h"
#include "gfx/window_state.h"
#include "utils/vec.h"

namespace gfx::ui {

struct Widget {
    virtual ~Widget() = default;

    virtual DrawList draw(double delta) = 0;

    virtual void on_key(const WindowEvent::KeyEvent& event) = 0;
    virtual void on_mouse_button_(const WindowEvent::MouseButtonEvent& event, utils::Vec2f pos) = 0;

    const utils::Transform2f& margin_transform() const;

    void on_mouse_button(const WindowEvent::MouseButtonEvent& event, utils::Vec2f pos);
    void on_window_event(const WindowEvent& event, const WindowState& state);

    void set_margin(float top, float right, float bottom, float left);

private:
    utils::Transform2f margin_tranform_{};
};

}  // namespace gfx::ui

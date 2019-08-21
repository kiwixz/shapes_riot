#pragma once

#include "gfx/draw_list.h"
#include "gfx/transform.h"
#include "gfx/window_event.h"
#include "gfx/window_state.h"
#include "utils/vec.h"

namespace gfx::ui {

struct Widget {
    virtual ~Widget() = default;

    virtual DrawList draw(double delta) = 0;

    virtual void on_key(const WindowEvent::KeyEvent& event) = 0;

    [[nodiscard]] const Transform3f& margin_transform() const;

    void on_mouse_button(const WindowEvent::MouseButtonEvent& event, utils::Vec3f pos);
    void on_window_event(const WindowEvent& event, const WindowState& state);

    void set_margin(float top, float right, float bottom, float left);

private:
    Transform3f margin_tranform_{};

    virtual void on_mouse_button_(const WindowEvent::MouseButtonEvent& event, utils::Vec3f pos) = 0;
};

}  // namespace gfx::ui

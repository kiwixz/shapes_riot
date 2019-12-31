#pragma once

#include "gfx/draw_list.h"
#include "gfx/transform.h"
#include "gfx/window_event.h"
#include "gfx/window_state.h"
#include "utils/vec.h"

namespace gfx::ui {

struct Widget {
    virtual ~Widget() = default;

    const Transform3f& margin_transform() const;

    DrawList draw(double delta, double aspect_ratio);
    void set_margin(float top, float right, float bottom, float left);

    void on_key(const WindowEvent::KeyEvent& event);
    void on_mouse_button(const WindowEvent::MouseButtonEvent& event, utils::Vec3f pos);
    void on_window_event(const WindowEvent& event, const WindowState& state);

protected:
    Widget() = default;
    Widget(const Widget&) = default;
    Widget& operator=(const Widget&) = default;
    Widget(Widget&&) = default;
    Widget& operator=(Widget&&) = default;

private:
    Transform3f margin_tranform_{};

    virtual void on_key_impl(const WindowEvent::KeyEvent& event) = 0;
    virtual void on_mouse_button_impl(const WindowEvent::MouseButtonEvent& event, utils::Vec3f pos) = 0;
    virtual DrawList draw_impl(double delta, double aspect_ratio) = 0;
};

}  // namespace gfx::ui

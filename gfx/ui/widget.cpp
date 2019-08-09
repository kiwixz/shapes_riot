#include "gfx/ui/widget.h"

namespace gfx::ui {

const utils::Transform2f& Widget::margin_tranform() const
{
    return margin_tranform_;
}

void Widget::on_window_event(const WindowEvent& event, const WindowState& state)
{
    if (const auto* key_event = event.as<gfx::WindowEvent::KeyEvent>())
        on_key(*key_event);
    else if (const auto* mouse_button_event = event.as<gfx::WindowEvent::MouseButtonEvent>())
        on_mouse_button(*mouse_button_event, state.mouse_pos);
}

void Widget::set_margin(float top, float right, float bottom, float left)
{
    margin_tranform_ = {utils::Vec2f{left - right, bottom - top} / 2.0f,
                        -utils::Vec2f{left + right, bottom + top} / 2.0f + 1.0f};
}

}  // namespace gfx::ui

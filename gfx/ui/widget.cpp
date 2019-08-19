#include "gfx/ui/widget.h"

namespace gfx::ui {

const Transform3f& Widget::margin_transform() const
{
    return margin_tranform_;
}

void Widget::on_mouse_button(const WindowEvent::MouseButtonEvent& event, utils::Vec3f pos)
{
    utils::Vec3f rel_pos = pos * margin_transform().inverse();
    if (rel_pos.x >= -1.0f && rel_pos.x <= 1.0f
        && rel_pos.y >= -1.0f && rel_pos.y <= 1.0f)
        on_mouse_button_(event, rel_pos);
}

void Widget::on_window_event(const WindowEvent& event, const WindowState& state)
{
    if (const auto* key_event = event.as<gfx::WindowEvent::KeyEvent>())
        on_key(*key_event);
    else if (const auto* mouse_button_event = event.as<gfx::WindowEvent::MouseButtonEvent>())
        on_mouse_button(*mouse_button_event, {state.mouse_pos, 0.0f});
}

void Widget::set_margin(float top, float right, float bottom, float left)
{
    margin_tranform_ = {utils::Vec3f{left - right, bottom - top, 0.0f} / 2.0f,
                        1.0f - utils::Vec3f{left + right, bottom + top, 0.0f} / 2.0f};
}

}  // namespace gfx::ui

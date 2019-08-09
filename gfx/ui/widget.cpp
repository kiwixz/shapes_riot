#include "gfx/ui/widget.h"

namespace gfx::ui {

void Widget::on_window_event(const WindowEvent& event, const WindowState& state)
{
    if (const auto* key_event = event.as<gfx::WindowEvent::KeyEvent>())
        on_key(*key_event);
    else if (const auto* mouse_button_event = event.as<gfx::WindowEvent::MouseButtonEvent>())
        on_mouse_button(*mouse_button_event, state.mouse_pos);
}

}  // namespace gfx::ui

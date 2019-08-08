#include "gfx/ui/vertical_layout.h"

namespace gfx::ui {

void VerticalLayout::draw(double delta)
{}

void VerticalLayout::on_key(const WindowEvent::KeyEvent& event)
{}

void VerticalLayout::on_mouse_button(const WindowEvent::KeyEvent& event, utils::Vec2d pos)
{}

void VerticalLayout::add_widget(std::unique_ptr<Widget>&& widget)
{
    children_.push_back({std::move(widget)});
}

}  // namespace gfx::ui

#include "gfx/ui/vertical_layout.h"

namespace gfx::ui {

DrawList VerticalLayout::draw(double delta)
{
    DrawList draw_list;
    for (const Child& child : children_) {
        DrawList child_draw_list = child.widget->draw(delta);
        child_draw_list.transform({{0.0f, child.position}, {1.0f, child.size}});
        draw_list.push(child_draw_list);
    }
    return draw_list;
}

void VerticalLayout::on_key(const WindowEvent::KeyEvent& event)
{}

void VerticalLayout::on_mouse_button(const WindowEvent::MouseButtonEvent& event, utils::Vec2d pos)
{}

void VerticalLayout::add_widget(std::unique_ptr<Widget>&& widget)
{
    children_.push_back({std::move(widget), 0.0f, 0.0f});
    for (size_t i = 0; i < children_.size(); ++i) {
        children_[i].size = 1.0f / children_.size();
        children_[i].position = i * children_[i].size;
    }
}

}  // namespace gfx::ui

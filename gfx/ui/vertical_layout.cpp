#include "gfx/ui/vertical_layout.h"

namespace gfx::ui {

DrawList VerticalLayout::draw(double delta)
{
    DrawList draw_list;
    for (const Child& child : children_) {
        DrawList child_draw_list = child.widget->draw(delta);
        child_draw_list.transform(child.transform * margin_transform());
        draw_list.push(child_draw_list);
    }
    return draw_list;
}

void VerticalLayout::on_key(const WindowEvent::KeyEvent& event)
{
    if (focus_)
        focus_->on_key(event);
}

void VerticalLayout::on_mouse_button_(const WindowEvent::MouseButtonEvent& event, utils::Vec2f pos)
{
    for (Child& child : children_) {
        utils::Vec2f rel_pos = pos * -child.transform;
        if (rel_pos.x >= -1.0f && rel_pos.x <= 1.0f
            && rel_pos.y >= -1.0f && rel_pos.y <= 1.0f) {
            focus_ = child.widget.get();
            child.widget->on_mouse_button(event, rel_pos);
            break;
        }
    }
}

void VerticalLayout::add_widget(std::unique_ptr<Widget>&& widget)
{
    children_.push_back({std::move(widget), {}});
    if (!focus_)
        focus_ = children_.front().widget.get();
    for (size_t i = 0; i < children_.size(); ++i) {
        float size = 2.0f / children_.size();
        children_[i].transform = {{0.0f, size / 2.0f + (children_.size() - 1 - i) * size - 1.0f},
                                  {1.0f, size / 2.0f}};
    }
}

}  // namespace gfx::ui

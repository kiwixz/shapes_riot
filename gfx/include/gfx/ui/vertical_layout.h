#pragma once

#include "gfx/ui/widget.h"
#include <memory>
#include <vector>

namespace gfx::ui {

struct VerticalLayout : Widget {
    DrawList draw(double delta) override;

    void on_key(const WindowEvent::KeyEvent& event) override;
    void on_mouse_button(const WindowEvent::MouseButtonEvent& event, utils::Vec2d pos) override;

    void add_widget(std::unique_ptr<Widget>&& widget);

private:
    struct Child {
        std::unique_ptr<Widget> widget;
        float position;
        float size;
    };

    std::vector<Child> children_;
    Child* focus_ = nullptr;
};

}  // namespace gfx::ui

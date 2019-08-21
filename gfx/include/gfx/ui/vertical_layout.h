#pragma once

#include "gfx/transform.h"
#include "gfx/ui/widget.h"
#include <memory>
#include <vector>

namespace gfx::ui {

struct VerticalLayout : Widget {
    DrawList draw(double delta) override;

    void on_key(const WindowEvent::KeyEvent& event) override;

    void add_widget(std::unique_ptr<Widget>&& widget);

private:
    struct Child {
        std::unique_ptr<Widget> widget;
        Transform3f transform;
    };

    std::vector<Child> children_;
    Widget* focus_ = nullptr;

    void on_mouse_button_(const WindowEvent::MouseButtonEvent& event, utils::Vec3f pos) override;
};

}  // namespace gfx::ui

#pragma once

#include "gfx/font.h"
#include "gfx/ui/widget.h"
#include "utils/resource_manager.h"

namespace gfx::ui {

struct Button : Widget {
    Button(utils::ResourceManager& resource_manager, std::string label, std::function<void()> on_click);

    void on_key(const WindowEvent::KeyEvent& event) override;

private:
    utils::ResourceHandle<Font> font_;
    std::string label_;
    std::function<void()> on_click_;

    void on_mouse_button_impl(const WindowEvent::MouseButtonEvent& event, utils::Vec3f pos) override;
    DrawList draw_impl(double delta, double aspect_ratio) override;
};

}  // namespace gfx::ui

#pragma once

#include "gfx/font.h"
#include "gfx/ui/widget.h"
#include "utils/resource_manager.h"

namespace gfx::ui {

struct Button : Widget {
    Button(utils::ResourceManager& resource_manager, std::string label, std::function<void()> on_click);

    DrawList draw(double delta) override;

    void on_key(const WindowEvent::KeyEvent& event) override;
    void on_mouse_button_(const WindowEvent::MouseButtonEvent& event, utils::Vec3f pos) override;

private:
    utils::ResourceHandle<Font> font_;
    std::string label_;
    std::function<void()> on_click_;
};

}  // namespace gfx::ui

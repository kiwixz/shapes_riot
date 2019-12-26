#pragma once

#include <string>

#include "gfx/font.h"
#include "gfx/ui/widget.h"
#include "utils/resource_manager.h"

namespace gfx::ui {

struct Label : Widget {
    Label(utils::ResourceHandle<Font> font, std::string label);

private:
    utils::ResourceHandle<Font> font_;
    std::string label_;

    void on_key_impl(const WindowEvent::KeyEvent& event) override;
    void on_mouse_button_impl(const WindowEvent::MouseButtonEvent& event, utils::Vec3f pos) override;
    DrawList draw_impl(double delta, double aspect_ratio) override;
};

}  // namespace gfx::ui

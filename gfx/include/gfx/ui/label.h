#pragma once

#include <string>

#include "gfx/anchor.h"
#include "gfx/font.h"
#include "gfx/ui/widget.h"
#include "utils/resource_manager.h"

namespace gfx::ui {

struct Label : Widget {
    Label() = default;
    Label(utils::ResourceHandle<Font> font, float size, Anchor anchor);
    Label(std::string text, utils::ResourceHandle<Font> font, float size, Anchor anchor);

    void set_text(std::string&& text);

private:
    utils::ResourceHandle<Font> font_;
    std::string text_;
    float size_;
    Anchor anchor_;

    void on_key_impl(const WindowEvent::KeyEvent& event) override;
    void on_mouse_button_impl(const WindowEvent::MouseButtonEvent& event, utils::Vec3f pos) override;
    DrawList draw_impl(double delta, double aspect_ratio) override;
};

}  // namespace gfx::ui

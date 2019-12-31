#include "gfx/ui/label.h"

namespace gfx::ui {

Label::Label(utils::ResourceHandle<Font> font, float size, Anchor anchor) :
    Label{"", std::move(font), size, anchor}
{}

Label::Label(std::string text, utils::ResourceHandle<Font> font, float size, Anchor anchor) :
    font_{std::move(font)}, text_{std::move(text)}, size_{size}, anchor_{anchor}
{}

void Label::set_text(std::string&& text)
{
    text_ = std::move(text);
}

void Label::on_key_impl(const WindowEvent::KeyEvent& /*event*/)
{}

void Label::on_mouse_button_impl(const WindowEvent::MouseButtonEvent& /*event*/, utils::Vec3f /*pos*/)
{}

DrawList Label::draw_impl(double aspect_ratio) const
{
    utils::Vec3f origin;

    if (anchor_ & Anchor::left)
        origin.x = -1.0f;
    else if (anchor_ & Anchor::right)
        origin.x = 1.0f;

    if (anchor_ & Anchor::top)
        origin.y = 1.0f;
    else if (anchor_ & Anchor::bottom)
        origin.y = -1.0f;

    return font_->draw_text(text_, origin, {static_cast<float>(size_ / aspect_ratio), size_}, anchor_);
}

}  // namespace gfx::ui

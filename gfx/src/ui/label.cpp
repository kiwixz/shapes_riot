#include "gfx/ui/label.h"

namespace gfx::ui {

Label::Label(utils::ResourceHandle<Font> font, std::string label) :
    font_{std::move(font)}, label_{std::move(label)}
{}

void Label::on_key_impl(const WindowEvent::KeyEvent& /*event*/)
{}

void Label::on_mouse_button_impl(const WindowEvent::MouseButtonEvent& /*event*/, utils::Vec3f /*pos*/)
{}

DrawList Label::draw_impl(double /*delta*/, double aspect_ratio)
{
    float size = 0.05f;
    utils::Vec3f pen{-1.0f, 1 - size, 0.5f};
    DrawList draw_list;
    draw_list += font_->draw_text_linear(label_, pen, {static_cast<float>(size / aspect_ratio), size});
    return draw_list;
}

}  // namespace gfx::ui

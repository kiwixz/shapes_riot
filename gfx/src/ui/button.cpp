#include "gfx/ui/button.h"
#include "embed/source_sans_pro.h"

namespace gfx::ui {

Button::Button(utils::ResourceManager& resource_manager, std::string label, std::function<void()> on_click) :
    label_{std::move(label)}, on_click_{std::move(on_click)}
{
    resource_manager.get_to("gfx_button_font", font_, [] {
        return std::make_shared<Font>(' ', '~' - ' ' + 1, embed::source_sans_pro(), 96);
    });
}

void Button::on_key(const WindowEvent::KeyEvent& event)
{
    if (event.action == GLFW_RELEASE && event.mods == 0
        && (event.key == GLFW_KEY_ENTER || event.key == GLFW_KEY_SPACE))
        on_click_();
}

void Button::on_mouse_button_impl(const WindowEvent::MouseButtonEvent& event, utils::Vec3f /*pos*/)
{
    if (event.action == GLFW_RELEASE && event.mods == 0 && event.button == GLFW_MOUSE_BUTTON_LEFT)
        on_click_();
}

DrawList Button::draw_impl(double /*delta*/, double aspect_ratio)
{
    static constexpr utils::Vec4f color = {0.0f, 0.0f, 0.0f, 0.85f};

    DrawList draw_list;
    draw_list.push_quad({{-1.0f, -1.0f, 0.0f}, color},
                        {{-1.0f, 1.0f, 0.0f}, color},
                        {{1.0f, 1.0f, 0.0f}, color},
                        {{1.0f, -1.0f, 0.0f}, color});
    draw_list += font_->draw_text(label_, {0.0f, 0.0f, 0.5f},
                                  {static_cast<float>(0.8f / aspect_ratio), 0.8f});
    return draw_list;
}

}  // namespace gfx::ui

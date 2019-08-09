#include "gfx/ui/button.h"
#include <array>

namespace gfx::ui {

Button::Button(std::string label, std::function<void()> on_click) :
    label_{std::move(label)}, on_click_{std::move(on_click)}
{}

DrawList Button::draw(double /*delta*/)
{
    static constexpr utils::Vec4f color = {0.0f, 0.0f, 0.0f, 0.9f};
    static constexpr std::array<gfx::Vertex, 4> vertices = {{{{-1.0f, -1.0f}, color},
                                                             {{-1.0f, 1.0f}, color},
                                                             {{1.0f, 1.0f}, color},
                                                             {{1.0f, -1.0f}, color}}};
    static constexpr std::array<gfx::Vertex::Index, 6> indexes = {{0, 1, 3, 1, 3, 2}};

    DrawList draw_list;
    draw_list.push(vertices, indexes);
    draw_list.transform(margin_transform());
    return draw_list;
}

void Button::on_key(const WindowEvent::KeyEvent& event)
{
    if (event.action == GLFW_RELEASE && event.mods == 0
        && (event.key == GLFW_KEY_ENTER || event.key == GLFW_KEY_SPACE))
        on_click_();
}

void Button::on_mouse_button_(const WindowEvent::MouseButtonEvent& event, utils::Vec2f /*pos*/)
{
    if (event.action == GLFW_RELEASE && event.mods == 0 && event.button == GLFW_MOUSE_BUTTON_LEFT)
        on_click_();
}

}  // namespace gfx::ui

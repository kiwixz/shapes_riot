#include "gfx/ui/button.h"
#include <array>

namespace gfx::ui {

Button::Button()
{}

DrawList Button::draw(double delta)
{
    static constexpr utils::Vec4f color = {0.0f, 0.0f, 0.0f, 0.9f};
    static constexpr std::array<gfx::DrawList::ColoredVertex, 4> vertices = {{{{-1.0f, -1.0f}, color},
                                                                              {{-1.0f, 1.0f}, color},
                                                                              {{1.0f, 1.0f}, color},
                                                                              {{1.0f, -1.0f}, color}}};
    static constexpr std::array<gfx::DrawList::Index, 6> indexes = {{0, 1, 3, 1, 3, 2}};

    DrawList draw_list;
    draw_list.push(vertices, indexes);
    return draw_list;
}

void Button::on_key(const WindowEvent::KeyEvent& event)
{}

void Button::on_mouse_button(const WindowEvent::KeyEvent& event, utils::Vec2d pos)
{}

}  // namespace gfx::ui

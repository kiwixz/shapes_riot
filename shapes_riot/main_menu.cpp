#include "main_menu.h"
#include "gfx/draw_list.h"
#include "gfx/ui/button.h"
#include <glad/glad.h>

namespace shapes_riot {

MainMenu::MainMenu(ScreenStack& screens) :
    screens_{&screens}
{
    layout_.set_margin(0.6f, 0.85f, 0.6f, 0.85f);
    layout_.add_widget(std::make_unique<gfx::ui::Button>("hello", std::bind(printf, "hello\n")));
}

void MainMenu::tick(double delta, const gfx::WindowState& state)
{
    glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    std::vector<gfx::Vertex> vertices;
    vertices.push_back({{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}});
    vertices.push_back({{0.0f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}});
    vertices.push_back({{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}});

    gfx::DrawList draw_list;
    draw_list.push(vertices);
    draw_list.push(layout_.draw(delta));
    drawer_.draw(draw_list);
}

void MainMenu::on_focus()
{}

void MainMenu::on_unfocus()
{}

void MainMenu::on_window_event(const gfx::WindowEvent& event, const gfx::WindowState& state)
{
    layout_.on_window_event(event, state);
}

}  // namespace shapes_riot

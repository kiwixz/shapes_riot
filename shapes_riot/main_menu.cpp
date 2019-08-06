#include "main_menu.h"
#include "gfx/draw_list.h"
#include <glad/glad.h>

namespace shapes_riot {

MainMenu::MainMenu(ScreenStack& screens) :
    screens_{&screens}
{}

void MainMenu::tick(double delta)
{
    glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    std::vector<gfx::DrawList::ColoredVertex> vertices;
    vertices.push_back({{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}});
    vertices.push_back({{0.0f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}});
    vertices.push_back({{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}});

    gfx::DrawList draw_list;
    draw_list.push(vertices);
    draw_list.draw();
}

void MainMenu::window_event(gfx::WindowEvent&& event)
{}

void MainMenu::on_focus()
{}

void MainMenu::on_unfocus()
{}

}  // namespace shapes_riot

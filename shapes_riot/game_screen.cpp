#include "game_screen.h"
#include "gfx/draw_list.h"
#include <glad/glad.h>

namespace shapes_riot {

GameScreen::GameScreen(ScreenStack& screens, utils::ResourceManager& resource_manager) :
    screens_{&screens}, drawer_{resource_manager}
{}

void GameScreen::tick(double /*delta*/, const gfx::WindowState& state)
{
    glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    player_.set_angle(std::atan2(state.mouse_pos.y, state.mouse_pos.x));

    gfx::DrawList draw_list;
    draw_list += player_.draw();
    drawer_.draw(draw_list);
}

void GameScreen::on_focus()
{}

void GameScreen::on_unfocus()
{}

void GameScreen::on_window_event(const gfx::WindowEvent& /*event*/, const gfx::WindowState& /*state*/)
{}

}  // namespace shapes_riot

#include "game_screen.h"
#include "gfx/draw_list.h"
#include <glad/glad.h>

namespace shapes_riot {

GameScreen::GameScreen(ScreenStack& screens, utils::ResourceManager& resource_manager) :
    screens_{&screens}, drawer_{resource_manager}
{}

void GameScreen::tick(double delta, const gfx::WindowState& state)
{
    glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    player_.set_angle(std::atan2(state.mouse_pos.y, state.mouse_pos.x));
    player_.tick(delta);
    camera_.center_on(player_.position());
    enemy_manager_.tick(delta, camera_.view(), player_.position());

    gfx::DrawList draw_list;
    draw_list += enemy_manager_.draw();
    draw_list += player_.draw();
    draw_list.transform(camera_.matrix());
    drawer_.draw(draw_list);
}

void GameScreen::on_focus()
{}

void GameScreen::on_unfocus()
{}

void GameScreen::on_window_event(const gfx::WindowEvent& event, const gfx::WindowState& /*state*/)
{
    if (const auto* key_event = event.as<gfx::WindowEvent::KeyEvent>())
        if (key_event->mods == 0 && (key_event->action == GLFW_PRESS || key_event->action == GLFW_RELEASE)) {
            if (key_event->key == GLFW_KEY_W)
                player_.add_acceleration({0.0, key_event->action == GLFW_PRESS ? 1.0 : -1.0});
            else if (key_event->key == GLFW_KEY_D)
                player_.add_acceleration({key_event->action == GLFW_PRESS ? 1.0 : -1.0, 0.0});
            else if (key_event->key == GLFW_KEY_S)
                player_.add_acceleration({0.0, key_event->action == GLFW_PRESS ? -1.0 : 1.0});
            else if (key_event->key == GLFW_KEY_A)
                player_.add_acceleration({key_event->action == GLFW_PRESS ? -1.0 : 1.0, 0.0});
        }
}

}  // namespace shapes_riot

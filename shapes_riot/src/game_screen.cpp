#include "game_screen.h"

#include <glad/glad.h>

#include "gfx/draw_list.h"

namespace shapes_riot {

GameScreen::GameScreen(ScreenStack& screens, utils::ResourceManager& resource_manager) :
    screens_{&screens}, drawer_{resource_manager}
{}

void GameScreen::tick(double delta, const gfx::WindowState& state)
{
    player_.set_angle(std::atan2(state.mouse_pos.y, state.mouse_pos.x * camera_.ratio()));
    player_.tick(delta, bullet_manager_);
    camera_.center_on(player_.position());
    map_.tick(delta, camera_.view());
    enemy_manager_.tick(delta, camera_.view(), player_.position());
    bullet_manager_.tick(delta, camera_.view());

    for (Bullet& bullet : bullet_manager_) {
        for (Enemy& enemy : enemy_manager_) {
            if (bullet.hitbox().is_colliding(enemy.hitbox()))
                bullet.on_collision(enemy);
        }
    }

    gfx::DrawList draw_list;
    draw_list += map_.draw();
    draw_list += bullet_manager_.draw();
    draw_list += enemy_manager_.draw();
    draw_list += player_.draw();
    draw_list.transform(camera_.matrix());
    draw_list += hud_.draw(delta, state, enemy_manager_);
    drawer_.draw(draw_list);
}

void GameScreen::on_focus(const gfx::WindowState& state)
{
    glClearColor(0.1f, 0.0f, 0.0f, 1.0f);

    camera_.set_ratio(static_cast<double>(state.framebuffer_size.x) / state.framebuffer_size.y);
}

void GameScreen::on_unfocus(const gfx::WindowState& /*state*/)
{}

void GameScreen::on_window_event(const gfx::WindowEvent& event, const gfx::WindowState& /*state*/)
{
    if (const auto* resize = event.as<gfx::WindowEvent::FramebufferResize>()) {
        camera_.set_ratio(static_cast<double>(resize->width) / resize->height);
    }
    else if (const auto* key_event = event.as<gfx::WindowEvent::KeyEvent>()) {
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
    else if (const auto* mouse_event = event.as<gfx::WindowEvent::MouseButtonEvent>()) {
        if (mouse_event->mods == 0 && mouse_event->button == GLFW_MOUSE_BUTTON_LEFT)
            player_.set_is_shooting(mouse_event->action == GLFW_PRESS);
    }
}

}  // namespace shapes_riot

#include "main_menu.h"

#include <glad/glad.h>

#include "gfx/draw_list.h"
#include "gfx/ui/button.h"

#include "game_screen.h"

namespace shapes_riot {

MainMenu::MainMenu(ScreenStack& screens, utils::ResourceManager& resource_manager) :
    drawer_{resource_manager}
{
    layout_.set_margin(0.7f, 0.7f, 0.7f, 0.7f);

    auto add_button = [&](std::string label, std::function<void()> on_click) {
        auto button = std::make_unique<gfx::ui::Button>(resource_manager, std::move(label), std::move(on_click));
        button->set_margin(0.05f, 0.0f, 0.05f, 0.0f);
        layout_.add_widget(std::move(button));
    };

    add_button("Play", [screens_ = &screens, resource_manager_ = &resource_manager] {
        screens_->emplace<GameScreen>(*resource_manager_);
    });
    add_button("Quit", [screens_ = &screens] { screens_->pop(); });
}

void MainMenu::tick(double delta, const gfx::WindowState& state)
{
    glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    gfx::DrawList draw_list;
    draw_list.push_triangle({{-0.9f, -0.9f, -0.9f}, {1.0f, 0.0f, 0.0f, 1.0f}},
                            {{0.0f, 0.9f, -0.9f}, {0.0f, 1.0f, 0.0f, 1.0f}},
                            {{0.9f, -0.9f, -0.9f}, {0.0f, 0.0f, 1.0f, 1.0f}});
    draw_list += layout_.draw(delta, static_cast<double>(state.framebuffer_size.x) / state.framebuffer_size.y);
    drawer_.draw(draw_list);
}

void MainMenu::on_focus(const gfx::WindowState& /*state*/)
{}

void MainMenu::on_unfocus(const gfx::WindowState& /*state*/)
{}

void MainMenu::on_window_event(const gfx::WindowEvent& event, const gfx::WindowState& state)
{
    layout_.on_window_event(event, state);
}

}  // namespace shapes_riot

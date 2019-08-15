#include "main_menu.h"
#include "embed/source_sans_pro.h"
#include "gfx/draw_list.h"
#include "gfx/font.h"
#include "gfx/ui/button.h"
#include <glad/glad.h>

namespace shapes_riot {

MainMenu::MainMenu(ScreenStack& screens, utils::ResourceManager& resource_manager) :
    screens_{&screens}, drawer_{resource_manager}
{
    layout_.set_margin(0.7f, 0.85f, 0.7f, 0.85f);

    auto add_button = [&](std::string label, std::function<void()> on_click) {
        auto button = std::make_unique<gfx::ui::Button>(std::move(label), std::move(on_click));
        button->set_margin(0.05f, 0.0f, 0.05f, 0.0f);
        layout_.add_widget(std::move(button));
    };

    add_button("hello", std::bind(printf, "hello\n"));
    add_button("world", std::bind(printf, "world\n"));
}

void MainMenu::tick(double delta, const gfx::WindowState& /*state*/)
{
    glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    static constexpr std::array<gfx::Vertex, 3> vertices = {{{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
                                                             {{0.0f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
                                                             {{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}}}};

    static gfx::Font font{'a', 'z' - 'a' + 1, embed::source_sans_pro(), 96};
    static constexpr std::array<gfx::Vertex, 4> font_vertices = {{{{-0.9f, -0.9f}, {0.0f, 1.0f}},
                                                                  {{-0.9f, 0.9f}, {0.8f, 0.8f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                                                                  {{0.9f, 0.9f}, {1.0f, 0.0f}},
                                                                  {{0.9f, -0.9f}, {1.0f, 1.0f}}}};
    static constexpr std::array<gfx::Vertex::Index, 6> font_indexes = {{0, 1, 3, 1, 3, 2}};

    gfx::DrawList draw_list;
    draw_list.push(vertices);
    draw_list.push(layout_.draw(delta));
    draw_list.push(font.texture(), font_vertices, font_indexes);
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

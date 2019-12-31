#include "hud.h"

#include "embed/source_sans_pro.h"

namespace shapes_riot {

Hud::Hud()
{
    auto font = std::make_shared<gfx::Font>(' ', '~' - ' ' + 1, embed::source_sans_pro(), 64);
    score_label_ = {font, 0.05f, gfx::Anchor::top_left};
    score_label_.set_margin(0.02f, 0.0f, 0.0f, 0.01f);
}

gfx::DrawList Hud::draw(double delta, const gfx::WindowState& state, const EnemyManager& enemy_manager)
{
    score_label_.set_text(fmt::format("Score: {}", enemy_manager.killed()));

    gfx::DrawList draw_list;
    draw_list += score_label_.draw(delta, static_cast<double>(state.framebuffer_size.x) / state.framebuffer_size.y);
    return draw_list;
}

}  // namespace shapes_riot

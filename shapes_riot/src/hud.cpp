#include "hud.h"

#include "embed/source_sans_pro.h"
#include "gfx/ui/label.h"

namespace shapes_riot {

Hud::Hud()
{
    font_ = std::make_shared<gfx::Font>(' ', '~' - ' ' + 1, embed::source_sans_pro(), 96);
}

gfx::DrawList Hud::draw(double delta, const gfx::WindowState& state, const EnemyManager& enemy_manager) const
{
    gfx::DrawList draw_list;
    gfx::ui::Label score_label{font_, fmt::format("Score: {}", enemy_manager.killed())};
    draw_list += score_label.draw(delta, static_cast<double>(state.framebuffer_size.x) / state.framebuffer_size.y);
    return draw_list;
}

}  // namespace shapes_riot

#pragma once

#include "gfx/draw_list.h"
#include "gfx/font.h"
#include "gfx/ui/label.h"
#include "gfx/window_state.h"
#include "utils/resource_manager.h"

#include "enemy_manager.h"

namespace shapes_riot {

struct Hud {
    Hud();

    gfx::DrawList draw(double delta, const gfx::WindowState& state, const EnemyManager& enemy_manager);

private:
    gfx::ui::Label score_label_;
};

}  // namespace shapes_riot

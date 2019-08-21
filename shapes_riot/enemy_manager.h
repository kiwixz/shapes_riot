#pragma once

#include "enemy.h"
#include "gfx/draw_list.h"
#include "rectangle.h"
#include "utils/rand.h"
#include <vector>

namespace shapes_riot {

struct EnemyManager {
    [[nodiscard]] gfx::DrawList draw() const;

    void tick(double delta, const Rectangle& camera_view, utils::Vec2d player_pos);

private:
    utils::RandF rand_;
    std::vector<Enemy> enemies_;
};

}  // namespace shapes_riot

#pragma once

#include <vector>

#include "gfx/draw_list.h"
#include "utils/logger.h"
#include "utils/rand.h"

#include "box.h"
#include "enemy.h"

namespace shapes_riot {

struct EnemyManager {
    std::vector<Enemy>::iterator begin();
    std::vector<Enemy>::iterator end();

    gfx::DrawList draw() const;
    int killed() const;

    void tick(double delta, const Box& camera_view, utils::Vec2d player_pos);

private:
    utils::Logger logger_{"EnemyManager"};
    utils::RandF rand_;
    std::vector<Enemy> enemies_;
    int killed_ = 0;
};

}  // namespace shapes_riot

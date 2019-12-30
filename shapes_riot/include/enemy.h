#pragma once

#include "gfx/draw_list.h"
#include "utils/vec.h"

#include "hitbox.h"

namespace shapes_riot {

struct Enemy {
    Enemy(utils::Vec2d pos, int level);

    gfx::DrawList draw() const;
    Hitbox hitbox() const;
    bool is_alive() const;

    void tick(double delta, utils::Vec2d target);
    void hurt(double hp);

private:
    utils::Vec2d pos_;
    utils::Vec2d velocity_;
    double angle_ = 0.0;
    double speed_ = 1.0;
    double hp_;

    int get_armor() const;
};

}  // namespace shapes_riot

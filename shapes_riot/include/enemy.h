#pragma once

#include "gfx/draw_list.h"
#include "hitbox.h"
#include "utils/vec.h"

namespace shapes_riot {

struct Enemy {
    Enemy(utils::Vec2d pos);

    gfx::DrawList draw() const;
    Hitbox hitbox() const;
    bool is_alive() const;

    void tick(double delta, utils::Vec2d target);
    void hurt(int hp);

private:
    utils::Vec2d pos_;
    utils::Vec2d velocity_;
    double angle_ = 0.0;
    int hp_ = 1;
};

}  // namespace shapes_riot

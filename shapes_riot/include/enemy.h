#pragma once

#include "gfx/draw_list.h"
#include "utils/vec.h"

#include "hitbox.h"

namespace shapes_riot {

inline constexpr int max_enemy_hp = 2;


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
    int hp_ = max_enemy_hp;
};

}  // namespace shapes_riot

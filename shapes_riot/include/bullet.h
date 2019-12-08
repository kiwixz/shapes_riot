#pragma once

#include "gfx/draw_list.h"
#include "utils/vec.h"

#include "enemy.h"
#include "hitbox.h"

namespace shapes_riot {

struct Bullet {
    Bullet(utils::Vec2d pos, double velocity, double angle);

    gfx::DrawList draw() const;
    utils::Vec2d position() const;
    Hitbox hitbox() const;

    void tick(double delta);
    void on_collision(Enemy& enemy);

private:
    utils::Vec2d pos_;
    utils::Vec2d velocity_;
    double angle_ = 0.0;
};

}  // namespace shapes_riot

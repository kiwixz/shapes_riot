#pragma once

#include "gfx/draw_list.h"
#include "utils/vec.h"

namespace shapes_riot {

struct Enemy {
    Enemy(utils::Vec2d pos);

    gfx::DrawList draw() const;

    void tick(double delta, utils::Vec2d target);

private:
    utils::Vec2d pos_;
    utils::Vec2d velocity_;
    double angle_ = 0.0;
};

}  // namespace shapes_riot

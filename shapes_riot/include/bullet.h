#pragma once

#include "gfx/draw_list.h"
#include "utils/vec.h"

namespace shapes_riot {

struct Bullet {
    Bullet(utils::Vec2d pos, double velocity, double angle);

    [[nodiscard]] gfx::DrawList draw() const;
    [[nodiscard]] utils::Vec2d position() const;

    void tick(double delta);

private:
    utils::Vec2d pos_;
    utils::Vec2d velocity_;
    double angle_ = 0.0;
};

}  // namespace shapes_riot

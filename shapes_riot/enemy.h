#pragma once

#include "gfx/draw_list.h"
#include "utils/vec.h"

namespace shapes_riot {

struct Enemy {
    [[nodiscard]] gfx::DrawList draw() const;

    void tick(double delta);

private:
    utils::Vec2d pos_;
    utils::Vec2d velocity_;
    utils::Vec2d acceleration_;
    double angle_ = 0.0;
};

}  // namespace shapes_riot

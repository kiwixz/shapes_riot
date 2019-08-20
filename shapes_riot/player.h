#pragma once

#include "gfx/draw_list.h"
#include "utils/vec.h"

namespace shapes_riot {

struct Player {
    [[nodiscard]] gfx::DrawList draw() const;

    utils::Vec2d position() const;

    void add_acceleration(utils::Vec2d vec);
    void set_angle(double angle);
    void tick(double delta);

private:
    utils::Vec2d pos_;
    utils::Vec2d velocity_;
    utils::Vec2d acceleration_;
    double angle_;
};

}  // namespace shapes_riot

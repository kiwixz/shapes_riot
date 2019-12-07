#pragma once

#include "utils/vec.h"

namespace shapes_riot {

struct Hitbox {
    Hitbox(utils::Vec2d pos, double radius);

    bool is_colliding(const Hitbox& other) const;

private:
    utils::Vec2d pos_;
    double radius_;
};

}  // namespace shapes_riot

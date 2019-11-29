#pragma once

#include "bullet_manager.h"

namespace shapes_riot {

struct Weapon {
    void fire(utils::Vec2d position, double angle, BulletManager& bullet_manager);
};

}  // namespace shapes_riot

#pragma once

#include "gfx/draw_list.h"
#include "gfx/window_state.h"
#include "utils/vec.h"

#include "bullet_manager.h"
#include "weapon.h"

namespace shapes_riot {

struct Player {
    gfx::DrawList draw() const;
    utils::Vec2d position() const;

    void add_acceleration(utils::Vec2d vec);
    void set_angle(double angle);
    void set_is_shooting(bool is_shooting);
    void tick(double delta, BulletManager& bullet_manager);

private:
    utils::Vec2d pos_;
    utils::Vec2d velocity_;
    utils::Vec2d acceleration_;
    double angle_ = 0.0;

    Weapon weapon_;
    bool is_shooting_ = false;
};

}  // namespace shapes_riot

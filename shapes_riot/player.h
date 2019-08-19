#pragma once

#include "gfx/draw_list.h"
#include "utils/matrix.h"
#include "utils/vec.h"

namespace shapes_riot {

struct Player {
    gfx::DrawList draw() const;

    void set_angle(float angle);

private:
    utils::Vec2f pos_;
    float angle_;
};

}  // namespace shapes_riot

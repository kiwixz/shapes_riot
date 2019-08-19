#include "player.h"

namespace shapes_riot {

gfx::DrawList Player::draw() const
{
    gfx::DrawList draw_list;
    draw_list.push_triangle({{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
                            {{0.0f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
                            {{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}});
    return draw_list;
}

void Player::set_angle(float angle)
{
    angle_ = angle;
}

}  // namespace shapes_riot

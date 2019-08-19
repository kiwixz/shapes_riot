#include "player.h"
#include "gfx/transformation.h"
#include "utils/matrix.h"

namespace shapes_riot {

gfx::DrawList Player::draw() const
{
    gfx::DrawList draw_list;
    utils::Matrix4f matrix = gfx::rotation(utils::Vec3f{0.0f, 0.0f, -angle_});
    draw_list.push_triangle({matrix * utils::Vec4f{-0.05f, 0.05f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
                            {matrix * utils::Vec4f{+0.05f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
                            {matrix * utils::Vec4f{-0.05f, -0.05f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}});
    return draw_list;
}

void Player::set_angle(float angle)
{
    angle_ = angle;
}

}  // namespace shapes_riot

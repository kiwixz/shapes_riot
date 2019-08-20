#include "player.h"
#include "gfx/transformation.h"
#include "utils/matrix.h"

namespace shapes_riot {

gfx::DrawList Player::draw() const
{
    gfx::DrawList draw_list;
    utils::Matrix4f matrix = gfx::rotation(utils::Vec3f{0.0f, 0.0f, static_cast<float>(-angle_)});
    draw_list.push_triangle({matrix * utils::Vec4f{-0.05f, 0.05f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
                            {matrix * utils::Vec4f{+0.05f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
                            {matrix * utils::Vec4f{-0.05f, -0.05f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}});
    return draw_list;
}

utils::Vec2d Player::position() const
{
    return pos_;
}

void Player::add_acceleration(utils::Vec2d vec)
{
    acceleration_ += vec;
}

void Player::set_angle(double angle)
{
    angle_ = angle;
}

void Player::tick(double delta)
{
    velocity_ += acceleration_ * delta * 20.0;
    acceleration_ /= 1.0 + delta * 20.0;
    pos_ += velocity_ * delta;

    printf("%.2f %.2f\n", pos_.x, pos_.y);
}

}  // namespace shapes_riot

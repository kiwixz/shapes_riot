#include "player.h"

#include "gfx/transformation.h"
#include "utils/matrix.h"

namespace shapes_riot {

gfx::DrawList Player::draw() const
{
    constexpr float size = 0.25f;

    gfx::DrawList draw_list;
    draw_list.push_triangle({utils::Vec4f{-size, size, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
                            {utils::Vec4f{size, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}},
                            {utils::Vec4f{-size, -size, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}});
    draw_list.transform(gfx::transformation<float>({utils::Vec2f{pos_}, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, static_cast<float>(-angle_)}));
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

void Player::set_is_shooting(bool is_shooting)
{
    is_shooting_ = is_shooting;
}

void Player::tick(double delta, BulletManager& bullet_manager)
{
    static constexpr double acceleration_ratio = 8.0;

    velocity_ += acceleration_ * acceleration_ratio * delta;
    pos_ += velocity_ * delta;
    velocity_ /= 1.0 + delta * acceleration_ratio;

    if (is_shooting_) {
        weapon_.fire(pos_, angle_, bullet_manager);
        is_shooting_ = false;
    }
}

}  // namespace shapes_riot

#include "enemy.h"

#include "gfx/transformation.h"
#include "utils/matrix.h"

namespace shapes_riot {

constexpr float size = 0.2f;


Enemy::Enemy(utils::Vec2d pos) :
    pos_{pos}
{}

gfx::DrawList Enemy::draw() const
{
    utils::Vec4f color{1.0f, 1.0f - static_cast<float>(hp_) / max_enemy_hp, 0.0f, 1.0f};

    gfx::DrawList draw_list;
    draw_list.push_triangle({utils::Vec4f{-size, size, 0.0f}, color},
                            {utils::Vec4f{size, 0.0f, 0.0f}, color},
                            {utils::Vec4f{-size, -size, 0.0f}, color});
    draw_list.transform(gfx::transformation<float>({utils::Vec2f{pos_}, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, static_cast<float>(-angle_)}));
    return draw_list;
}

Hitbox Enemy::hitbox() const
{
    return {pos_, size};
}

bool Enemy::is_alive() const
{
    return hp_ > 0;
}

void Enemy::tick(double delta, utils::Vec2d target)
{
    static constexpr double acceleration_ratio = 8.0;

    utils::Vec2d acceleration = target - pos_;  // go towards target
    acceleration.normalize();
    angle_ = std::atan2(acceleration.y, acceleration.x);

    velocity_ += acceleration * acceleration_ratio * delta;
    pos_ += velocity_ * delta;
    velocity_ /= 1.0 + delta * acceleration_ratio;
}

void Enemy::hurt(int hp)
{
    hp_ = std::max(hp_ - hp, 0);
}

}  // namespace shapes_riot

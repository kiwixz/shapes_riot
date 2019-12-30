#include "enemy.h"

#include "gfx/transformation.h"
#include "utils/math.h"
#include "utils/matrix.h"

namespace shapes_riot {
namespace {

constexpr float layer_size = 0.15f;
constexpr float outer_layer_size = 0.3f;

}  // namespace


Enemy::Enemy(utils::Vec2d pos, int level) :
    pos_{pos}, speed_{1.0 / (level + 1)}, hp_{static_cast<double>(level + 1)}
{}

gfx::DrawList Enemy::draw() const
{
    int armor = get_armor();

    gfx::DrawList draw_list;
    auto triangle = [&](float size, utils::Vec4f color) {
        constexpr float x_offset = 1.0f / 3.0f;  // this offset ensures the triangle centroid is (0;0)

        float xos = x_offset * size;
        return draw_list.push_triangle({utils::Vec4f{-size + xos, size, 0.0f}, color},
                                       {utils::Vec4f{size + xos, 0.0f, 0.0f}, color},
                                       {utils::Vec4f{-size + xos, -size, 0.0f}, color});
    };

    triangle(armor * layer_size + outer_layer_size,
             {1.0f,
              static_cast<float>(1.0 - (hp_ - armor)),
              static_cast<float>(1.0 - 1.0 / speed_),
              1.0f});

    for (int layer = armor - 1; layer >= 0; --layer) {
        triangle(layer_size * (layer + 1),
                 {static_cast<float>(1.0 - 1.0 / speed_),
                  1.0f / (armor - layer),
                  1.0f,
                  1.0f});
    }

    draw_list.transform(gfx::transformation<float>({utils::Vec2f{pos_}, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, static_cast<float>(-angle_)}));
    return draw_list;
}

Hitbox Enemy::hitbox() const
{
    return {pos_, outer_layer_size + get_armor() * layer_size};
}

bool Enemy::is_alive() const
{
    return hp_ > 0;
}

void Enemy::tick(double delta, utils::Vec2d target)
{
    constexpr double acceleration_ratio = 100.0;
    constexpr double deceleration_ratio = 50.0;
    constexpr double time_acceleration_ratio = 0.1;

    utils::Vec2d acceleration = target - pos_;  // go towards target
    acceleration.normalize();
    angle_ = std::atan2(acceleration.y, acceleration.x);

    velocity_ += acceleration * acceleration_ratio * speed_ * delta;
    pos_ += velocity_ * delta;
    velocity_ /= 1.0 + delta * deceleration_ratio;
    speed_ *= 1.0 + delta * time_acceleration_ratio;
}

void Enemy::hurt(double hp)
{
    hp_ = std::max(hp_ - hp, 0.0);
}

int Enemy::get_armor() const
{
    int armor = static_cast<int>(hp_);
    if (armor == hp_)  // we are max hp
        return armor - 1;
    else
        return armor;
}

}  // namespace shapes_riot

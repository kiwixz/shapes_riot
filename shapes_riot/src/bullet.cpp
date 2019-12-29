#include "bullet.h"

#include <cmath>

#include "gfx/transformation.h"
#include "utils/matrix.h"

namespace shapes_riot {
namespace {

constexpr utils::Vec2f size{0.05f, 0.02f};

}


Bullet::Bullet(utils::Vec2d pos, double velocity, double angle) :
    pos_{pos},
    velocity_{std::cos(angle) * velocity, std::sin(angle) * velocity},
    angle_{angle}
{}

gfx::DrawList Bullet::draw() const
{
    constexpr utils::Vec4f color{1.0f, 1.0f, 1.0f, 1.0f};

    gfx::DrawList draw_list;
    draw_list.push_quad({utils::Vec4f{-size, 0.0f}, color},
                        {utils::Vec4f{-size.x, size.y, 0.0f}, color},
                        {utils::Vec4f{size, 0.0f}, color},
                        {utils::Vec4f{size.x, -size.y, 0.0f}, color});
    draw_list.transform(gfx::transformation<float>({utils::Vec2f{pos_}, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, static_cast<float>(-angle_)}));
    return draw_list;
}

utils::Vec2d Bullet::position() const
{
    return pos_;
}

Hitbox Bullet::hitbox() const
{
    return {pos_, size.y};
}

bool Bullet::is_alive() const
{
    return is_alive_;
}

void Bullet::tick(double delta)
{
    pos_ += velocity_ * delta;
}

void Bullet::on_collision(Enemy& enemy)
{
    is_alive_ = false;
    enemy.hurt(1);
}

}  // namespace shapes_riot

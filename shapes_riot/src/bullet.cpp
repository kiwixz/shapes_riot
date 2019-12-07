#include "bullet.h"
#include "gfx/transformation.h"
#include "utils/matrix.h"
#include <cmath>

namespace shapes_riot {

Bullet::Bullet(utils::Vec2d pos, double velocity, double angle) :
    pos_{pos},
    velocity_{std::cos(angle) * velocity, std::sin(angle) * velocity},
    angle_{angle}
{}

gfx::DrawList Bullet::draw() const
{
    static constexpr utils::Vec2f size{0.02f, 0.005f};
    static constexpr utils::Vec4f color{1.0f, 1.0f, 1.0f, 1.0f};

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
    return {pos_, 0.01};
}

void Bullet::tick(double delta)
{
    pos_ += velocity_ * delta;
}

void Bullet::on_collision(Enemy& enemy)
{
    enemy.hurt(1);
}

}  // namespace shapes_riot

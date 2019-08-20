#include "enemy.h"
#include "gfx/transformation.h"
#include "utils/matrix.h"

namespace shapes_riot {

gfx::DrawList Enemy::draw() const
{
    gfx::DrawList draw_list;
    draw_list.push_triangle({utils::Vec4f{-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
                            {utils::Vec4f{+0.5f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
                            {utils::Vec4f{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}});
    draw_list.transform(gfx::transformation<float>({utils::Vec2f{pos_}, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, static_cast<float>(-angle_)}));
    return draw_list;
}

void Enemy::tick(double delta)
{
    static constexpr double acceleration_ratio = 8.0;

    velocity_ += acceleration_ * acceleration_ratio * delta;
    pos_ += velocity_ * delta;
    velocity_ /= 1.0 + delta * acceleration_ratio;
}

}  // namespace shapes_riot
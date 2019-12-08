#include "hitbox.h"

#include "utils/math.h"

namespace shapes_riot {

Hitbox::Hitbox(utils::Vec2d pos, double radius) :
    pos_{pos}, radius_{radius}
{}

bool Hitbox::is_colliding(const Hitbox& other) const
{
    return utils::pow2(other.pos_.x - pos_.x) + utils::pow2(other.pos_.y - pos_.y) < utils::pow2(radius_ + other.radius_);
}

}  // namespace shapes_riot

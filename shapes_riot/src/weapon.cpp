#include "weapon.h"

namespace shapes_riot {

void Weapon::fire(utils::Vec2d position, double angle, BulletManager& bullet_manager)
{
    bullet_manager.add({position, 8.0, angle});
}

}  // namespace shapes_riot

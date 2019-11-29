#include "bullet_manager.h"
#include <cmath>

namespace shapes_riot {

gfx::DrawList BulletManager::draw() const
{
    gfx::DrawList draw_list;
    for (const Bullet& bullet : bullets_)
        draw_list += bullet.draw();
    return draw_list;
}

void BulletManager::add(Bullet bullet)
{
    bullets_.push_back(std::move(bullet));
}

void BulletManager::tick(double delta, const Box& camera_view)
{
    for (auto it = bullets_.begin(); it != bullets_.end();) {
        it->tick(delta);

        // delete bullet if its too far away
        utils::Vec2d position_in_view = (it->position() - camera_view.center) / camera_view.half_size;
        if (std::abs(position_in_view.x) > 3.0
            || std::abs(position_in_view.y) > 3.0)
            it = bullets_.erase(it);
        else
            ++it;
    }
}

}  // namespace shapes_riot

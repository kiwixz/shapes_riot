#include "enemy_manager.h"

namespace shapes_riot {

std::vector<Enemy>::iterator EnemyManager::begin()
{
    return enemies_.begin();
}

std::vector<Enemy>::iterator EnemyManager::end()
{
    return enemies_.end();
}

gfx::DrawList EnemyManager::draw() const
{
    gfx::DrawList draw_list;
    for (const Enemy& enemy : enemies_)
        draw_list += enemy.draw();
    return draw_list;
}

void EnemyManager::tick(double delta, const Box& camera_view, utils::Vec2d player_pos)
{
    if (rand_() < delta * 2) {
        // generate random pos either [-1.0; -0.5[ or [0.5; 1.0]
        utils::Vec2d pos{rand_(), rand_()};
        if (pos.x < 0.5)
            pos.x = -(pos.x + 0.5);
        if (pos.y < 0.5)
            pos.y = -(pos.y + 0.5);

        // place pos randomly out of camera view
        pos = camera_view.center + pos * 3.0 * camera_view.half_size;

        enemies_.emplace_back(pos);
    }

    for (auto it = enemies_.begin(); it != enemies_.end();) {
        Enemy& enemy = *it;
        if (enemy.is_alive()) {
            enemy.tick(delta, player_pos);
            ++it;
        }
        else {
            it = enemies_.erase(it);
        }
    }
}

}  // namespace shapes_riot

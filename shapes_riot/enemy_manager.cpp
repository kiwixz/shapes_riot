#include "enemy_manager.h"

namespace shapes_riot {

gfx::DrawList EnemyManager::draw() const
{
    gfx::DrawList draw_list;
    for (const Enemy& enemy : enemies_)
        draw_list += enemy.draw();
    return draw_list;
}

void EnemyManager::tick(double delta, const Box& camera_view, utils::Vec2d player_pos)
{
    if (rand_.gen() < delta * 2) {
        // generate random pos either [-1.0; -0.5[ or [0.5; 1.0]
        utils::Vec2d pos{rand_.gen(), rand_.gen()};
        if (pos.x < 0.5)
            pos.x = -(pos.x + 0.5);
        if (pos.y < 0.5)
            pos.y = -(pos.y + 0.5);

        // place pos randomly out of camera view
        pos = camera_view.center + pos * 3.0 * camera_view.half_size;

        enemies_.emplace_back(pos);
    }

    for (Enemy& enemy : enemies_)
        enemy.tick(delta, player_pos);
}

}  // namespace shapes_riot

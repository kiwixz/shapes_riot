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

int EnemyManager::killed() const
{
    return killed_;
}

void EnemyManager::tick(double delta, const Box& camera_view, utils::Vec2d player_pos)
{
    constexpr double spawn_delay = 3.0;
    constexpr double kills_for_double_spawn = 1000.0;
    constexpr int kills_armor_scale = 8;

    if (rand_() < delta / spawn_delay * (1.0 + killed_ / kills_for_double_spawn)) {
        // generate random pos either [-1.2; -1.1[ or [1.1; 1.2]
        utils::Vec2d pos{rand_() / 5, rand_() / 5};
        if (pos.x < 0.1)
            pos.x = -(pos.x + 1.1);
        else
            pos.x += 1.0;
        if (pos.y < 0.1)
            pos.y = -(pos.y + 1.1);
        else
            pos.y += 1.0;

        // place pos randomly out of camera view
        pos = camera_view.center + pos * camera_view.half_size;

        int level = static_cast<int>(std::sqrt(rand_() * killed_ / kills_armor_scale + 1.0) - 1.0);
        logger_(utils::LogLevel::info, "spawning enemy level {} at ({},{})", level, pos.x, pos.y);
        enemies_.emplace_back(pos, level);
    }

    for (auto it = enemies_.begin(); it != enemies_.end();) {
        Enemy& enemy = *it;
        if (enemy.is_alive()) {
            enemy.tick(delta, player_pos);
            ++it;
        }
        else {
            it = enemies_.erase(it);
            ++killed_;
            logger_(utils::LogLevel::info, "killed one enemy, total: {}", killed_);
        }
    }
}

}  // namespace shapes_riot

#pragma once

#include "box.h"
#include "bullet.h"
#include "gfx/draw_list.h"
#include "utils/rand.h"
#include <vector>

namespace shapes_riot {

struct BulletManager {
    [[nodiscard]] gfx::DrawList draw() const;

    void add(Bullet bullet);
    void tick(double delta, const Box& camera_view);

private:
    std::vector<Bullet> bullets_;
};

}  // namespace shapes_riot

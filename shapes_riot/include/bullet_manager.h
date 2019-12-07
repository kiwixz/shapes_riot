#pragma once

#include "box.h"
#include "bullet.h"
#include "gfx/draw_list.h"
#include "utils/rand.h"
#include <vector>

namespace shapes_riot {

struct BulletManager {
    std::vector<Bullet>::iterator begin();
    std::vector<Bullet>::iterator end();

    gfx::DrawList draw() const;

    void add(Bullet bullet);
    void tick(double delta, const Box& camera_view);

private:
    std::vector<Bullet> bullets_;
};

}  // namespace shapes_riot

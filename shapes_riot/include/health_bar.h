#pragma once

#include "gfx/draw_list.h"

namespace shapes_riot {

struct HealthBar {
    HealthBar(int hp, int max_hp);

    [[nodiscard]] gfx::DrawList draw() const;

    void tick(double delta, int hp);

private:
    int hp_;
    int max_hp_;
};

}  // namespace shapes_riot

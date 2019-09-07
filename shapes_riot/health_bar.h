#pragma once

#include "gfx/draw_list.h"

namespace shapes_riot {

struct HealthBar {
    HealthBar() = default;
    HealthBar(double hp);

    [[nodiscard]] gfx::DrawList draw() const;

    void tick(double delta, double hp);

private:
    double hp_ = 1.0;
};

}  // namespace shapes_riot

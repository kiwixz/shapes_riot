#pragma once

#include "gfx/draw_list.h"
#include "gfx/font.h"
#include "gfx/ui/label.h"
#include "gfx/window_state.h"
#include "utils/resource_manager.h"

namespace shapes_riot {

struct Perf {
    Perf();

    gfx::DrawList draw(const gfx::WindowState& state) const;

    void tick(double delta);

private:
    gfx::ui::Label fps_label_;
    double time_ = 0.0;
    std::vector<double> second_deltas_;
};

}  // namespace shapes_riot

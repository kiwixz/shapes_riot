#pragma once

#include "gfx/draw_list.h"
#include "gfx/font.h"
#include "gfx/window_state.h"
#include "utils/resource_manager.h"

namespace shapes_riot {

struct Perf {
    Perf();

    gfx::DrawList draw(double delta, const gfx::WindowState& state) const;

    void tick(double delta);

private:
    utils::ResourceHandle<gfx::Font> font_;
    double time_ = 0.0;
    std::vector<double> second_deltas_;
    int last_fps_ = 0;
};

}  // namespace shapes_riot
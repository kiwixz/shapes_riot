#include "perf.h"

#include "embed/source_sans_pro.h"

namespace shapes_riot {

Perf::Perf()
{
    auto font = std::make_shared<gfx::Font>(' ', '~' - ' ' + 1, embed::source_sans_pro(), 64);
    fps_label_ = {font, 0.03f, gfx::Anchor::bottom_left};
    fps_label_.set_margin(0.0f, 0.0f, 0.01f, 0.005f);
}

gfx::DrawList Perf::draw(double delta, const gfx::WindowState& state)
{
    gfx::DrawList draw_list;
    draw_list += fps_label_.draw(delta, static_cast<double>(state.framebuffer_size.x) / state.framebuffer_size.y);
    return draw_list;
}

void Perf::tick(double delta)
{
    time_ += delta;
    if (time_ > 1.0) {
        time_ -= std::trunc(time_);
        fps_label_.set_text(fmt::format("{} fps", second_deltas_.size()));
        second_deltas_.clear();
    }
    second_deltas_.push_back(delta);
}

}  // namespace shapes_riot

#include "perf.h"

#include <numeric>
#include <ratio>

#include "embed/source_sans_pro.h"
#include "utils/math.h"

namespace shapes_riot {

Perf::Perf()
{
    auto font = std::make_shared<gfx::Font>(' ', '~' - ' ' + 1, embed::source_sans_pro(), 64);
    fps_label_ = {font, 0.03f, gfx::Anchor::bottom_left};
    fps_label_.set_margin(0.0f, 0.0f, 0.01f, 0.005f);
}

gfx::DrawList Perf::draw(const gfx::WindowState& state) const
{
    gfx::DrawList draw_list;
    draw_list += fps_label_.draw(static_cast<double>(state.framebuffer_size.x) / state.framebuffer_size.y);
    return draw_list;
}

void Perf::tick(double delta)
{
    constexpr double label_update_delay = 0.2;

    time_ += delta;
    if (time_ > label_update_delay) {
        time_ = 0;

        double sum = std::accumulate(second_deltas_.begin(), second_deltas_.end(), 0.0);
        double mean = sum / second_deltas_.size();

        double accum = std::accumulate(second_deltas_.begin(), second_deltas_.end(), 0.0,
                                       [&](double current, double a) {
                                           return current + utils::pow2(a - mean);
                                       });

        double dev = std::sqrt(accum / (second_deltas_.size() - 1));
        double fps = 1.0 / mean;
        fps_label_.set_text(fmt::format("{:.2f} fps  ({:.4f}ms ~{:.4f})", fps, mean * std::milli::den, dev * std::milli::den));
    }

    second_deltas_.push_back(delta);
}

}  // namespace shapes_riot

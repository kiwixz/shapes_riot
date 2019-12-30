#include "perf.h"

#include "embed/source_sans_pro.h"
#include "gfx/ui/label.h"

namespace shapes_riot {

Perf::Perf()
{
    font_ = std::make_shared<gfx::Font>(' ', '~' - ' ' + 1, embed::source_sans_pro(), 96);
}

gfx::DrawList Perf::draw(double delta, const gfx::WindowState& state) const
{
    gfx::DrawList draw_list;
    gfx::ui::Label fps_label{font_, fmt::format("{} fps", last_fps_)};
    draw_list += fps_label.draw(delta, static_cast<double>(state.framebuffer_size.x) / state.framebuffer_size.y);
    return draw_list;
}

void Perf::tick(double delta)
{
    time_ += delta;
    if (time_ > 1.0) {
        time_ -= std::trunc(time_);
        last_fps_ = second_deltas_.size();
        second_deltas_.clear();
    }
    second_deltas_.push_back(delta);
}

}  // namespace shapes_riot

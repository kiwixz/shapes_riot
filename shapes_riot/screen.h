#pragma once

#include "gfx/window_event.h"
#include "gfx/window_state.h"

namespace shapes_riot {

struct Screen {
    virtual ~Screen() = default;

    virtual void tick(double delta, const gfx::WindowState& state) = 0;

    virtual void on_focus() = 0;
    virtual void on_unfocus() = 0;
    virtual void on_window_event(const gfx::WindowEvent& event, const gfx::WindowState& state) = 0;
};

}  // namespace shapes_riot

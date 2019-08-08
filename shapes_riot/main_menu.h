#pragma once

#include "screen_stack.h"

namespace shapes_riot {

struct MainMenu : Screen {
    MainMenu() = default;
    explicit MainMenu(ScreenStack& screens);

    void tick(double delta, const gfx::WindowState& state) override;
    void window_event(const gfx::WindowEvent& event, const gfx::WindowState& state) override;

    void on_focus() override;
    void on_unfocus() override;

private:
    ScreenStack* screens_;
};

}  // namespace shapes_riot

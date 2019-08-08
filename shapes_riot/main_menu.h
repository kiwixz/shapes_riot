#pragma once

#include "gfx/drawer.h"
#include "gfx/ui/vertical_layout.h"
#include "screen_stack.h"

namespace shapes_riot {

struct MainMenu : Screen {
    MainMenu() = default;
    explicit MainMenu(ScreenStack& screens);

    void tick(double delta, const gfx::WindowState& state) override;

    void on_focus() override;
    void on_unfocus() override;
    void on_window_event(const gfx::WindowEvent& event, const gfx::WindowState& state) override;

private:
    ScreenStack* screens_;

    gfx::Drawer drawer_;
    gfx::ui::VerticalLayout layout_;
};

}  // namespace shapes_riot

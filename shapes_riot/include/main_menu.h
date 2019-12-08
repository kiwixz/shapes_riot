#pragma once

#include "gfx/drawer.h"
#include "gfx/ui/vertical_layout.h"
#include "utils/resource_manager.h"

#include "screen_stack.h"

namespace shapes_riot {

struct MainMenu : Screen {
    MainMenu(ScreenStack& screens, utils::ResourceManager& resource_manager);

    void tick(double delta, const gfx::WindowState& state) override;

    void on_focus(const gfx::WindowState& state) override;
    void on_unfocus(const gfx::WindowState& state) override;
    void on_window_event(const gfx::WindowEvent& event, const gfx::WindowState& state) override;

private:
    gfx::Drawer drawer_;
    gfx::ui::VerticalLayout layout_;
};

}  // namespace shapes_riot

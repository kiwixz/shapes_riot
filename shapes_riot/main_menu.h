#pragma once

#include "screen_stack.h"

namespace shapes_riot {

struct MainMenu : Screen {
    MainMenu() = default;
    explicit MainMenu(ScreenStack& screens);

    void tick(double delta) override;

    void on_focus() override;
    void on_unfocus() override;

private:
    ScreenStack* screens_;
};

}  // namespace shapes_riot

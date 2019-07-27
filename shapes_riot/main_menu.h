#pragma once

#include "screen_stack.h"

namespace shapes_riot {

struct MainMenu : Screen {
    MainMenu() = default;
    explicit MainMenu(ScreenStack& screens);

    virtual void tick(double delta) override;

    virtual void on_focus() override;
    virtual void on_unfocus() override;

private:
    ScreenStack* screens_;
};

}  // namespace shapes_riot

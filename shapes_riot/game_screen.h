#pragma once

#include "bullet_manager.h"
#include "camera.h"
#include "enemy_manager.h"
#include "gfx/drawer.h"
#include "player.h"
#include "screen_stack.h"
#include "utils/resource_manager.h"

namespace shapes_riot {

struct GameScreen : Screen {
    GameScreen(ScreenStack& screens, utils::ResourceManager& resource_manager);

    void tick(double delta, const gfx::WindowState& state) override;

    void on_focus() override;
    void on_unfocus() override;
    void on_window_event(const gfx::WindowEvent& event, const gfx::WindowState& state) override;

private:
    ScreenStack* screens_;

    gfx::Drawer drawer_;
    Camera camera_;
    Player player_;
    BulletManager bullet_manager_;
    EnemyManager enemy_manager_;
};

}  // namespace shapes_riot

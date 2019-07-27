#pragma once

namespace shapes_riot {

struct Screen {
    virtual ~Screen() = default;

    virtual void tick(double delta) = 0;

    virtual void on_focus() = 0;
    virtual void on_unfocus() = 0;
};

}  // namespace shapes_riot

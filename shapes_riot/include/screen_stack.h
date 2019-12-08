#pragma once

#include <memory>
#include <stack>
#include "gfx/window.h"
#include "screen.h"

namespace shapes_riot {

struct ScreenStack {
    ScreenStack(gfx::Window& window);

    bool empty() const;
    Screen& top() const;

    void pop();

    template <typename T, typename... Args>
    void emplace(Args&&... args);

private:
    gfx::Window* window_;
    std::stack<std::unique_ptr<Screen>> screens_;
};


template <typename T, typename... Args>
void ScreenStack::emplace(Args&&... args)
{
    if (!screens_.empty())
        screens_.top()->on_unfocus(window_->state());
    screens_.push(std::make_unique<T>(*this, std::forward<Args>(args)...));
    screens_.top()->on_focus(window_->state());
}

}  // namespace shapes_riot

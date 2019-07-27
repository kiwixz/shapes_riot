#pragma once

#include "screen.h"
#include <memory>
#include <stack>

namespace shapes_riot {

struct ScreenStack {
    [[nodiscard]] Screen& top() const;

    void pop();

    template <typename T, typename... Args>
    void emplace(Args&&... args);

private:
    std::stack<std::unique_ptr<Screen>> screens_;
};


template <typename T, typename... Args>
void ScreenStack::emplace(Args&&... args)
{
    if (!screens_.empty())
        screens_.top()->on_unfocus();
    screens_.push(std::make_unique<T>(*this, std::forward<Args>(args)...));
    screens_.top()->on_focus();
}

}  // namespace shapes_riot

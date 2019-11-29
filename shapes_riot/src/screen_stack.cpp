#include "screen_stack.h"

namespace shapes_riot {

ScreenStack::ScreenStack(gfx::Window& window) :
    window_{&window}
{}

bool ScreenStack::empty() const
{
    return screens_.empty();
}

Screen& ScreenStack::top() const
{
    return *screens_.top();
}

void ScreenStack::pop()
{
    screens_.top()->on_unfocus(window_->state());
    screens_.pop();
    if (!screens_.empty())
        screens_.top()->on_focus(window_->state());
}

}  // namespace shapes_riot

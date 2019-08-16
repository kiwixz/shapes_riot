#include "screen_stack.h"
#include "screen_stack.h"

namespace shapes_riot {

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
    screens_.top()->on_unfocus();
    screens_.pop();
    if (!screens_.empty())
        screens_.top()->on_focus();
}

}  // namespace shapes_riot

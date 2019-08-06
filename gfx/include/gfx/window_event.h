#pragma once

#include <variant>

namespace gfx {

struct WindowEvent {
    struct KeyEvent {
        int key;
        int scancode;
        int action;
        int mods;
    };

    template <typename T, std::enable_if_t<!std::is_same_v<T, WindowEvent>, int> = 0>
    explicit WindowEvent(T&& event);

    template <typename T>
    [[nodiscard]] T* as();

    template <typename T>
    [[nodiscard]] const T* as() const;

private:
    using Variant = std::variant<KeyEvent>;

    Variant variant_;
};


template <typename T, std::enable_if_t<!std::is_same_v<T, WindowEvent>, int>>
WindowEvent::WindowEvent(T&& event) :
    variant_{event}
{}

template <typename T>
T* WindowEvent::as()
{
    return std::get_if<T>(&variant_);
}

template <typename T>
const T* WindowEvent::as() const
{
    return std::get_if<T>(&variant_);
}

}  // namespace gfx

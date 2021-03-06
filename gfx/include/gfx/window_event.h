#pragma once

#include <variant>

namespace gfx {

struct WindowEvent {
    struct FramebufferResize {
        int width;
        int height;
    };

    struct KeyEvent {
        int key;
        int scancode;
        int action;
        int mods;
    };

    struct MouseButtonEvent {
        int button;
        int action;
        int mods;
    };

    template <typename T>
    explicit WindowEvent(T event);

    template <typename T>
    T* as();

    template <typename T>
    const T* as() const;

private:
    using Variant = std::variant<FramebufferResize,
                                 KeyEvent,
                                 MouseButtonEvent>;

    Variant variant_;
};


template <typename T>
WindowEvent::WindowEvent(T event) :
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

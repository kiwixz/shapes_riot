#include "gfx/window.h"
#include "main_menu.h"
#include "screen_stack.h"
#include "utils/exception.h"
#include "utils/resource_manager.h"
#include <fmt/printf.h>
#include <chrono>

namespace shapes_riot {
namespace {

int main(int /*argc*/, char** /*argv*/)
{
    try {
        gfx::GlfwHandle glfw_handle;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 8);
#ifdef DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

        gfx::Window window{{1600, 900}, "Shapes Riot"};
        glfwSwapInterval(1);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        utils::ResourceManager resource_manager;
        ScreenStack screens;
        screens.emplace<MainMenu>(resource_manager);

        using Clock = std::chrono::high_resolution_clock;
        Clock::time_point last_frame = Clock::now();

        while (!glfwWindowShouldClose(window.ptr())) {
            Clock::time_point now = Clock::now();
            double delta = std::chrono::duration<double>(now - last_frame).count();
            last_frame = now;

            gfx::WindowState state = window.state();
            window.poll_events([&](gfx::WindowEvent&& event) {
                if (const auto* key_event = event.as<gfx::WindowEvent::KeyEvent>()) {
                    if (key_event->action == GLFW_RELEASE
                        && key_event->mods == 0
                        && key_event->key == GLFW_KEY_ESCAPE)
                        glfwSetWindowShouldClose(window.ptr(), true);
                }

                if (const auto* framebuffer_resize = event.as<gfx::WindowEvent::FramebufferResize>())
                    glViewport(0, 0, framebuffer_resize->width, framebuffer_resize->height);

                screens.top().on_window_event(event, state);
            });
            screens.top().tick(delta, state);

            glfwSwapBuffers(window.ptr());
        }

        return 0;
    }
    catch (const utils::Exception& ex) {
        fmt::print(stderr, "fatal exception: {}\nfrom: {}\n", ex.what(), ex.from());
    }
    catch (const std::exception& ex) {
        fmt::print(stderr, "fatal exception: {}\n", ex.what());
    }
    return 1;
}

}  // namespace
}  // namespace shapes_riot


int main(int argc, char** argv)
{
    shapes_riot::main(argc, argv);
}

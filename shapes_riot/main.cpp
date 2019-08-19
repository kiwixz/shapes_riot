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

        gfx::Window window{{1, 1}, "Shapes Riot"};
        glfwSwapInterval(1);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glClearDepth(-1.0f);
        glDepthFunc(GL_GEQUAL);
        glEnable(GL_DEPTH_TEST);

        utils::ResourceManager resource_manager;
        ScreenStack screens;
        screens.emplace<MainMenu>(resource_manager);

        using Clock = std::chrono::high_resolution_clock;
        Clock::time_point last_frame = Clock::now();

        glfwSetWindowSize(window.ptr(), 1600, 900);

        while (!glfwWindowShouldClose(window.ptr())) {
            Clock::time_point now = Clock::now();
            double delta = std::chrono::duration<double>(now - last_frame).count();
            last_frame = now;

            gfx::WindowState state = window.state();
            window.poll_events([&](gfx::WindowEvent&& event) {
                if (const auto* framebuffer_resize = event.as<gfx::WindowEvent::FramebufferResize>()) {
                    if (framebuffer_resize->width > framebuffer_resize->height)
                        glViewport((framebuffer_resize->width - framebuffer_resize->height) / 2, 0,
                                   framebuffer_resize->height, framebuffer_resize->height);
                    else
                        glViewport(0, (framebuffer_resize->height - framebuffer_resize->width) / 2,
                                   framebuffer_resize->width, framebuffer_resize->width);
                }

                if (screens.empty())
                    return;
                screens.top().on_window_event(event, state);
            });
            if (screens.empty())
                break;
            glClear(GL_DEPTH_BUFFER_BIT);
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

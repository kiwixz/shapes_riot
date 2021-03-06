#include <chrono>
#include <iostream>

#include <fmt/printf.h>
#include <glad/glad.h>

#include "gfx/window.h"
#include "utils/exception.h"
#include "utils/logger.h"
#include "utils/os.h"
#include "utils/resource_manager.h"

#include "main_menu.h"
#include "perf.h"
#include "screen_stack.h"

int main(int /*argc*/, char** /*argv*/)
{
    using namespace shapes_riot;

    std::set_terminate(&utils::terminate);
    utils::set_thread_name("main_thread");

    gfx::Window window{"Shapes Riot", {1600, 900}};

    if (!GLAD_GL_ARB_direct_state_access)
        throw MAKE_EXCEPTION("missing gl extension ARB_direct_state_access");

    if (GLAD_GL_KHR_debug) {
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION,
                              0, nullptr, GL_FALSE);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei /*length*/, const GLchar* message, const void* /*userParam*/) {
            utils::Logger{"OpenGL"}(utils::LogLevel::info,
                                    "source:{:#x} type:{:#x} id:{:#x} severity:{:#x}\n\t {}",
                                    source, type, id, severity, message);
        },
                               nullptr);
    }
    else {
        utils::Logger{"OpenGL"}(utils::LogLevel::warning, "missing extension KHR_debug");
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glClearDepth(-1.0f);
    glDepthFunc(GL_GEQUAL);
    glEnable(GL_DEPTH_TEST);

    utils::ResourceManager resource_manager;
    ScreenStack screens{window};
    screens.emplace<MainMenu>(resource_manager);

    gfx::Drawer drawer{resource_manager};
    Perf perf;

    using Clock = std::chrono::steady_clock;
    Clock::time_point last_frame = Clock::now();

    while (!window.is_closed()) {
        Clock::time_point now = Clock::now();
        double delta = std::chrono::duration<double>(now - last_frame).count();
        last_frame = now;

        gfx::WindowState state = window.state();
        window.poll_events([&](gfx::WindowEvent&& event) {
            if (const auto* framebuffer_resize = event.as<gfx::WindowEvent::FramebufferResize>())
                glViewport(0, 0, framebuffer_resize->width, framebuffer_resize->height);
            screens.top().on_window_event(event, state);
        });
        if (screens.empty())
            break;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screens.top().tick(delta, state);

        perf.tick(delta);
        drawer.draw(perf.draw(state));

        window.swap_buffers();
    }

    return 0;
}

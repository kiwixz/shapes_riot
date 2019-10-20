#include "gfx/window.h"
#include "main_menu.h"
#include "screen_stack.h"
#include "utils/exception.h"
#include "utils/logger.h"
#include "utils/resource_manager.h"
#include <fmt/printf.h>
#include <chrono>

#include <iostream>

namespace shapes_riot {
namespace {

int main(int /*argc*/, char** /*argv*/)
{
    try {
        glfwSetErrorCallback([](int error, const char* description) {
            utils::Logger{"glfw"}(utils::LogLevel::error, "error {:#x}: {}", error, description);
        });

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

        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION,
                              0, nullptr, GL_FALSE);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei /*length*/, const GLchar* message, const void* /*userParam*/) {
            utils::Logger{"opengl"}(utils::LogLevel::info, "source:{:#x} type:{:#x} id:{:#x} severity:{:#x}\n\t {}",
                                    source, type, id, severity, message);
        },
                               nullptr);

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
                if (const auto* framebuffer_resize = event.as<gfx::WindowEvent::FramebufferResize>())
                    glViewport(0, 0, framebuffer_resize->width, framebuffer_resize->height);
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
        utils::Logger{""}(utils::LogLevel::critical, "fatal exception: {}\nfrom: {}", ex.what(), ex.from());
    }
    catch (const std::exception& ex) {
        utils::Logger{""}(utils::LogLevel::critical, "fatal exception: {}", ex.what());
    }
    return 1;
}

}  // namespace
}  // namespace shapes_riot


int main(int argc, char** argv)
{
    return shapes_riot::main(argc, argv);
}

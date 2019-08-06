#include "main_window.h"
#include "main_menu.h"
#include <chrono>

namespace shapes_riot {

MainWindow::MainWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    window_ = {{1600, 900}, "Shapes Riot"};

    glfwSwapInterval(1);
}

void MainWindow::loop()
{
    screens_.emplace<MainMenu>();

    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point last_frame = Clock::now();

    while (!glfwWindowShouldClose(window_.ptr())) {
        Clock::time_point now = Clock::now();
        double delta = std::chrono::duration<double>(now - last_frame).count();
        last_frame = now;

        window_.poll_events([&](gfx::WindowEvent&& event) {
            if (auto* key_event = event.as<gfx::WindowEvent::KeyEvent>()) {
                if (key_event->action == GLFW_RELEASE
                    && key_event->mods == 0
                    && key_event->key == GLFW_KEY_ESCAPE)
                    glfwSetWindowShouldClose(window_.ptr(), true);
            }
            screens_.top().window_event(std::move(event));
        });
        screens_.top().tick(delta);

        glfwSwapBuffers(window_.ptr());
    }
}

void MainWindow::on_key(int key, int /*scancode*/, int action, int mods)
{
    if (action == GLFW_RELEASE) {
        if (mods == 0) {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window_.ptr(), true);
        }
    }
}

}  // namespace shapes_riot

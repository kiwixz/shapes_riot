#include "main_window.h"
#include "main_menu.h"
#include <chrono>
#include <string_view>

namespace shapes_riot {

MainWindow::Glfw::Glfw()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
}


MainWindow::MainWindow() :
    window_{{1600, 900}, "Shapes Riot"}
{
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window_.ptr(), this);
    glfwSetKeyCallback(window_.ptr(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        return reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window))->on_key(key, scancode, action, mods);
    });

    constexpr std::string_view vertex_source = R"(
            #version 450
            layout(location=0) in vec2 pos;
            layout(location=1) in vec4 color;
            layout(location=0) out vec4 color_v;
            void main() {
                gl_Position = vec4(pos, 0.0f, 1.0f);
                color_v = color;
            }
        )";
    constexpr std::string_view fragment_source = R"(
            #version 450
            layout(location=0) in vec4 color_v;
            out vec4 color;
            void main() {
                //color = color_v;
                color = vec4( 1.0, 0.0, 0.0, 1.0 );
            }
        )";
    program_.attach({GL_VERTEX_SHADER, vertex_source});
    program_.attach({GL_FRAGMENT_SHADER, fragment_source});
    program_.link();
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

        glfwPollEvents();
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

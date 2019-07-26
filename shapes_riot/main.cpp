#include "gfx/window.h"

namespace shapes_riot {
namespace {

int main(int /*argc*/, char** /*argv*/)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    gfx::Window window{{1600, 900}, "Shapes Riot"};

    return 0;
}

}  // namespace
}  // namespace shapes_riot


int main(int argc, char** argv)
{
    shapes_riot::main(argc, argv);
}

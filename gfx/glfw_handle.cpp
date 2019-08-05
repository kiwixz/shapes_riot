#include "gfx/glfw_handle.h"
#include "utils/exception.h"

namespace gfx {

void glfw_handle_init()
{
    if (!glfwInit())
        throw MAKE_EXCEPTION("could not initialize glfw");
}

}  // namespace gfx

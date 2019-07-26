#include "gfx/glfw_handle.h"
#include "utils/exception.h"

namespace gfx {

void glfw_handle_init()
{
    if (!glfwInit())
        throw utils::Exception{"could not initialize glfw"};
}

}  // namespace gfx

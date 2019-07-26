#pragma once

#include "gfx/glad_glfw.h"
#include "utils/shared_handle.h"

namespace gfx {

void glfw_handle_init();

using GlfwHandle = utils::SharedHandle<glfw_handle_init, glfwTerminate>;

}  // namespace gfx

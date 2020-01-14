#pragma once

#include <GLFW/glfw3.h>

#include "utils/global_handle.h"

namespace gfx {

void glfw_handle_init();

using GlfwHandle = utils::GlobalHandle<glfw_handle_init, glfwTerminate>;

}  // namespace gfx

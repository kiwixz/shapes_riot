#pragma once

#include "gfx/glad_glfw.h"
#include "utils/global_handle.h"

namespace gfx {

void glfw_handle_init();

using GlfwHandle = utils::GlobalHandle<glfw_handle_init, glfwTerminate>;

}  // namespace gfx

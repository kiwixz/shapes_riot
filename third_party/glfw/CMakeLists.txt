cmake_minimum_required(VERSION 3.12)
include("third_party")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

third_party("glfw" REPO "glfw/glfw" TAG "3.3.2")
system_include_dirs("glfw")
target_compile_definitions("glfw" INTERFACE GLFW_INCLUDE_NONE)

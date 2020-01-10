cmake_minimum_required(VERSION 3.12)
include("third_party")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")

third_party("glfw" REPO "glfw/glfw" TAG "3.3")
system_include_dirs("glfw")
suppress_warnings("glfw")

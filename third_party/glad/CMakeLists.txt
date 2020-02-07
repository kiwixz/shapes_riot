cmake_minimum_required(VERSION 3.12)
include("third_party")

set(GLAD_API "gl=3.3" CACHE STRING "")
set(GLAD_EXTENSIONS "GL_KHR_debug,GL_ARB_direct_state_access" CACHE STRING "")
set(GLAD_PROFILE "core" CACHE STRING "")
set(GLAD_REPRODUCIBLE ON CACHE BOOL "")
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "")

third_party("glad" REPO "Dav1dde/glad" TAG "v0.1.33")
system_include_dirs("glad")

cmake_minimum_required(VERSION 3.12)
include("third_party")

set(GLAD_API "gl=4.5")
set(GLAD_PROFILE "core")
set(GLAD_REPRODUCIBLE ON)
set(USE_MSVC_RUNTIME_LIBRARY_DLL ON)

third_party("glad" "Dav1dde/glad" "v0.1.33")
system_include_dirs("glad")

cmake_minimum_required(VERSION 3.12)
include("${CMAKE_SOURCE_DIR}/cmake/install_vcpkg.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/set_defaults.cmake")


option(BUILD_TESTS "Build tests" ON)
option(USE_VCPKG "Use vcpkg toolchain to install and find libs" ON)


set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
set_defaults_tools_cpp()

if (USE_VCPKG)
    install_vcpkg("2019.09"
        LIBS
            # gfx
            "freetype"
            "glad"
            "glfw3"

            "doctest"
            "fmt"
    )
endif ()


project("shapes_riot" C CXX ASM_NASM)

set_defaults_options()
set_defaults_flags_asm()
set_defaults_flags_cpp()

if (BUILD_TESTS)
    enable_testing()
    find_package("Python3" REQUIRED COMPONENTS "Interpreter")
    add_test(NAME "check_clang_format"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/check_clang_format.py"
    )
    add_test(NAME "check_clang_tidy"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/check_clang_tidy.py" "${CMAKE_BINARY_DIR}"
    )
endif ()


add_subdirectory("gfx")
add_subdirectory("utils")

add_subdirectory("shapes_riot")

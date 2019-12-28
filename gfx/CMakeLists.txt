cmake_minimum_required(VERSION 3.12)
include("test_target_cpp")

set(target "gfx")
set(src
    draw_list
    drawer
    font
    glfw_handle
    shader
    texture
    texture_view
    window_state
    window

    ui/button
    ui/label
    ui/vertical_layout
    ui/widget
)
set(test
    transformation
)


list(TRANSFORM src PREPEND "src/")
list(TRANSFORM src APPEND ".cpp")
add_library("${target}" STATIC ${src})
target_include_directories("${target}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (BUILD_TESTING)
    list(TRANSFORM test PREPEND "test/")
    list(TRANSFORM test APPEND ".test.cpp")
    test_target_cpp("${target}" ${test})
endif ()


target_link_libraries("${target}"
    "utils"

    "freetype"
    "glad"
    "glfw"
)

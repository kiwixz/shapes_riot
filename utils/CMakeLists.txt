cmake_minimum_required(VERSION 3.12)
include("test_target_cpp")

set(target "utils")

file(GLOB_RECURSE src CONFIGURE_DEPENDS "src/*.cpp")
add_library("${target}" STATIC ${src})
target_include_directories("${target}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (BUILD_TESTING)
    file(GLOB_RECURSE test_src CONFIGURE_DEPENDS "test/*.cpp")
    test_target_cpp("${target}" ${test_src})
endif ()


target_link_libraries("${target}"
    "fmt::fmt"
)

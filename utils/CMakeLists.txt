cmake_minimum_required(VERSION 3.12)
include("${CMAKE_SOURCE_DIR}/cmake/test_target_cpp.cmake")

set(target "utils")

add_library("${target}"
    config.cpp
    exception.cpp
    paths.cpp
    rand.cpp
    resource_manager.cpp
    scope_exit.cpp
    thread_pool.cpp
)
target_include_directories("${target}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

test_target_cpp("${target}"
    bind_front.test.cpp
    config.test.cpp
    make_array.test.cpp
    math.test.cpp
    matrix.test.cpp
    resource_manager.test.cpp
    scope_exit.test.cpp
    shared_handle.test.cpp
    span.test.cpp
    thread_pool.test.cpp
    vec.test.cpp
)


find_package("fmt" CONFIG REQUIRED)

target_link_libraries("${target}"
    "fmt::fmt"
)

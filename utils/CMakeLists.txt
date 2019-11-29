cmake_minimum_required(VERSION 3.12)
include("test_target_cpp")
include("vcpkg")

set(target "utils")
set(src
    config
    exception
    logger
    logging
    os
    paths
    rand
    resource_manager
    scope_exit
    thread_pool
)
set(test
    bind_front
    config
    function_ref
    guarded
    make_array
    math
    matrix
    resource_manager
    scope_exit
    global_handle
    span
    static_function
    thread_pool
    vec
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


if (USE_VCPKG)
    vcpkg_install(LIBS
        "fmt"
    )
endif ()

find_package("fmt" CONFIG REQUIRED)

target_link_libraries("${target}"
    "fmt::fmt"
)

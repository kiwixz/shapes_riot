cmake_minimum_required(VERSION 3.12)
include("test_target_cpp")

set(target "utils")

add_library("${target}"
    config.cpp
    exception.cpp
    logger.cpp
    logging.cpp
    os.cpp
    paths.cpp
    rand.cpp
    resource_manager.cpp
    scope_exit.cpp
    thread_pool.cpp
)
target_include_directories("${target}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (BUILD_TESTS)
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
endif ()


find_package("fmt" CONFIG REQUIRED)

target_link_libraries("${target}"
    "fmt::fmt"
)

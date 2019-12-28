cmake_minimum_required(VERSION 3.12)
include("FetchContent")

set(target "fmt")

FetchContent_Declare("${target}"
    GIT_REPOSITORY "https://github.com/fmtlib/fmt"
    GIT_TAG "6.0.0"
)

FetchContent_GetProperties("${target}")
if (NOT "${target}_POPULATED")
    FetchContent_Populate("${target}")
    add_subdirectory("${${target}_SOURCE_DIR}" "${${target}_BINARY_DIR}")
endif()

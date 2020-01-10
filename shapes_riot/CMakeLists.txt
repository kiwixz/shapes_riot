cmake_minimum_required(VERSION 3.12)
include("download")
include("file_embed")
include("test_target_cpp")

set(target "shapes_riot")
set(target_lib "${target}_lib")

file(GLOB_RECURSE src CONFIGURE_DEPENDS "src/*.cpp")
add_library("${target_lib}" STATIC ${src})
target_include_directories("${target_lib}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable("${target}" "src/main.cpp")
target_link_libraries("${target}" "${target_lib}")

if (BUILD_TESTING)
    file(GLOB_RECURSE test_src CONFIGURE_DEPENDS "test/*.cpp")
    test_target_cpp("${target_lib}" ${test_src})
endif ()


download_github("source_sans_pro" "adobe-fonts/source-sans-pro" "2.045R-ro/1.095R-it" "OTF/SourceSansPro-Regular.otf")
file_embed("${target}" "source_sans_pro" "${source_sans_pro_PATH}")

target_link_libraries("${target_lib}"
    "gfx"
    "utils"

    "${source_sans_pro_EMBED}"

    "fmt::fmt"
)

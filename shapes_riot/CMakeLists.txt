cmake_minimum_required(VERSION 3.12)
include("${CMAKE_SOURCE_DIR}/cmake/download.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/file_embed.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/test_target_cpp.cmake")

set(target "shapes_riot")

file(GLOB_RECURSE src "*.cpp")
list(REMOVE_ITEM src "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
list(FILTER src EXCLUDE REGEX ".test.cpp$")

add_library("${target}_base" "${src}")
add_executable("${target}" "main.cpp")
target_link_libraries("${target}" "${target}_base")

file(GLOB_RECURSE test_src "*.test.cpp")
test_target_cpp("${target}_base" "${test_src}")


download_github("source_sans_pro" "adobe-fonts/source-sans-pro" "2.045R-ro/1.095R-it")
file_embed("shapes_riot" "source_sans_pro" "${source_sans_pro_DIR}/OTF/SourceSansPro-Regular.otf"
    DEPENDS "source_sans_pro")

find_package("fmt" CONFIG REQUIRED)

target_link_libraries("${target}_base"
    "${source_sans_pro_EMBED}"

    "gfx"
    "utils"

    "fmt::fmt"
)

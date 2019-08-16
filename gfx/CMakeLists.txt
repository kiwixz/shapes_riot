cmake_minimum_required(VERSION 3.12)
include("${CMAKE_SOURCE_DIR}/cmake/download.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/file_embed.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/test_target_cpp.cmake")

set(target "gfx")

add_library("${target}"
    draw_list.cpp
    drawer.cpp
    font.cpp
    glfw_handle.cpp
    shader.cpp
    texture.cpp
    window_state.cpp
    window.cpp

    ui/button.cpp
    ui/vertical_layout.cpp
    ui/widget.cpp
)
target_include_directories("${target}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

test_target_cpp("${target}"
)


download_github("source_sans_pro" "adobe-fonts/source-sans-pro" "2.045R-ro/1.095R-it")
file_embed("${target}" "source_sans_pro" "${source_sans_pro_DIR}/OTF/SourceSansPro-Regular.otf"
    DEPENDS "source_sans_pro")

find_package("Freetype" CONFIG REQUIRED)
find_package("glad" CONFIG REQUIRED)
find_package("glfw3" CONFIG REQUIRED)

target_link_libraries("${target}"
    "${source_sans_pro_EMBED}"

    "utils"

    "freetype"
    "glad::glad"
    "glfw"
)

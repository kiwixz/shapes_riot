cmake_minimum_required(VERSION 3.12)
include("download")
include("file_embed")
include("test_target_cpp")
include("vcpkg")

set(target "gfx")

add_library("${target}"
    draw_list.cpp
    drawer.cpp
    font.cpp
    glfw_handle.cpp
    shader.cpp
    texture.cpp
    texture_view.cpp
    window_state.cpp
    window.cpp

    ui/button.cpp
    ui/vertical_layout.cpp
    ui/widget.cpp
)
target_include_directories("${target}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (BUILD_TESTS)
    test_target_cpp("${target}"
        transformation.test.cpp
    )
endif ()


download_github("source_sans_pro" "adobe-fonts/source-sans-pro" "2.045R-ro/1.095R-it")
file_embed("${target}" "source_sans_pro"
    "${source_sans_pro_DIR}/OTF/SourceSansPro-Regular.otf"
    DEPENDS "source_sans_pro")


if (USE_VCPKG)
    vcpkg_install(LIBS
        "freetype"
        "glad"
        "glfw3"
    )
endif ()

find_package("Freetype" CONFIG REQUIRED)
find_package("glad" CONFIG REQUIRED)
find_package("glfw3" CONFIG REQUIRED)

target_link_libraries("${target}"
    "utils"

    "${source_sans_pro_EMBED}"

    "freetype"
    "glad::glad"
    "glfw"
)

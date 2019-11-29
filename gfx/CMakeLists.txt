cmake_minimum_required(VERSION 3.12)
include("download")
include("file_embed")
include("test_target_cpp")
include("vcpkg")

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

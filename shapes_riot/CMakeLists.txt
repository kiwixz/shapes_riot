cmake_minimum_required(VERSION 3.12)
include("download")
include("file_embed")
include("test_target_cpp")
include("vcpkg")

set(target "shapes_riot")
set(src
    bullet
    bullet_manager
    camera
    enemy
    enemy_manager
    game_screen
    health_bar
    hud
    hitbox
    main_menu
    player
    screen_stack
    weapon
)
set(test
)


list(TRANSFORM src PREPEND "src/")
list(TRANSFORM src APPEND ".cpp")
add_library("${target}_base" STATIC ${src})
target_include_directories("${target}_base" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable("${target}" src/main.cpp)
target_link_libraries("${target}" "${target}_base")

if (BUILD_TESTING)
    list(TRANSFORM test PREPEND "test/")
    list(TRANSFORM test APPEND ".test.cpp")
    test_target_cpp("${target}_base" ${test})
endif ()


download_github("source_sans_pro" "adobe-fonts/source-sans-pro" "2.045R-ro/1.095R-it")
file_embed("${target}" "source_sans_pro"
    "${source_sans_pro_DIR}/OTF/SourceSansPro-Regular.otf"
    DEPENDS "source_sans_pro")


target_link_libraries("${target}_base"
    "gfx"
    "utils"

    "${source_sans_pro_EMBED}"

    "fmt::fmt"
)

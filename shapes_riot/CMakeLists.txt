cmake_minimum_required(VERSION 3.12)
include("${CMAKE_SOURCE_DIR}/cmake/test_target_cpp.cmake")

set(target "shapes_riot")

add_library("${target}_base"
    bullet.cpp
    bullet_manager.cpp
    camera.cpp
    enemy.cpp
    enemy_manager.cpp
    game_screen.cpp
    main_menu.cpp
    player.cpp
    screen_stack.cpp
    weapon.cpp
)
add_executable("${target}" main.cpp)
target_link_libraries("${target}" "${target}_base")

test_target_cpp("${target}_base"
)


find_package("fmt" CONFIG REQUIRED)

target_link_libraries("${target}_base"
    "gfx"
    "utils"

    "fmt::fmt"
)

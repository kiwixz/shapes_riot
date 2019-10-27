cmake_minimum_required(VERSION 3.12)
include("test_target_cpp")
include("vcpkg")

set(target "shapes_riot")

add_library("${target}_base"
    bullet.cpp
    bullet_manager.cpp
    camera.cpp
    enemy.cpp
    enemy_manager.cpp
    game_screen.cpp
    health_bar.cpp
    main_menu.cpp
    player.cpp
    screen_stack.cpp
    weapon.cpp
)
add_executable("${target}" main.cpp)
target_link_libraries("${target}" "${target}_base")

if (BUILD_TESTS)
    test_target_cpp("${target}_base"
    )
endif ()


if (USE_VCPKG)
    vcpkg_install(LIBS
        "fmt"
    )
endif ()

find_package("fmt" CONFIG REQUIRED)

target_link_libraries("${target}_base"
    "gfx"
    "utils"

    "fmt::fmt"
)

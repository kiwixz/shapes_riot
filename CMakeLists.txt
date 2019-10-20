cmake_minimum_required(VERSION 3.12)
include("${CMAKE_SOURCE_DIR}/cmake/install_vcpkg.cmake")

option(BUILD_TESTS "Build tests" ON)
option(USE_VCPKG "Use vcpkg toolchain to install and find libs" ON)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
set(CMAKE_C_FLAGS "" CACHE STRING "")
set(CMAKE_CXX_FLAGS "" CACHE STRING "")
set(CMAKE_ASM_NASM_FLAGS "" CACHE STRING "")
if (WIN32)
    set(CMAKE_C_COMPILER "cl" CACHE FILEPATH "")
    set(CMAKE_CXX_COMPILER "cl" CACHE FILEPATH "")
else ()
    set(CMAKE_C_COMPILER "clang" CACHE FILEPATH "")
    set(CMAKE_CXX_COMPILER "clang++" CACHE FILEPATH "")
endif ()

install_vcpkg("2019.09"
    LIBS
        # gfx
        "freetype"
        "glad"
        "glfw3"

        "doctest"
        "fmt"
)

project("shapes_riot" C CXX ASM_NASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Weverything  \
        -Wno-c++98-compat-pedantic -Wno-ctad-maybe-unsupported -Wno-disabled-macro-expansion  \
        -Wno-double-promotion -Wno-exit-time-destructors -Wno-float-equal -Wno-padded  \
        -Wno-return-std-move-in-c++11 -Wno-sign-conversion -Wno-weak-vtables"
    )
    set(CMAKE_CXX_FLAGS_DEBUG "-D DEBUG -Og -g -fno-omit-frame-pointer -fsanitize=address,undefined")
    set(CMAKE_CXX_FLAGS_RELEASE "-D NDEBUG -O3 -flto -ffast-math")
    link_libraries("pthread" "stdc++fs")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /nologo /diagnostics:caret /bigobj /MP /W4 /permissive- /EHsc /fp:fast")
    set(CMAKE_CXX_FLAGS_DEBUG "/D DEBUG /MTd /GF /Oi /JMC /RTC1 /ZI")
    set(CMAKE_CXX_FLAGS_RELEASE "/D NDEBUG /MT /O2 /GL /Gw")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG:FASTLINK")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG:INCREMENTAL")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "/LTCG")
    link_libraries("onecore.lib")
endif ()

set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -Wall")
set(CMAKE_ASM_NASM_FLAGS_DEBUG "-g")
set(CMAKE_ASM_NASM_FLAGS_RELEASE "")
if (WIN32)
    set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -Xvc")
else ()
    set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -Xgnu")
endif ()

if (BUILD_TESTS)
    enable_testing()
    find_package("Python3" COMPONENTS "Interpreter")
    add_test(NAME "check_clang_format"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/check_clang_format.py"
    )
    add_test(NAME "check_clang_tidy"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/check_clang_tidy.py" "${CMAKE_BINARY_DIR}"
    )
endif ()


add_subdirectory("gfx")
add_subdirectory("utils")

add_subdirectory("shapes_riot")

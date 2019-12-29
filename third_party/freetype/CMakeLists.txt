cmake_minimum_required(VERSION 3.12)
include("third_party")

#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
#set(GLFW_BUILD_DOCS OFF CACHE BOOL "")

third_party("freetype" URL "https://download-mirror.savannah.gnu.org/releases/freetype/freetype-2.10.1.tar.xz"
    PATCHES
        "version.patch"
        "zlib_debug.patch"
)
system_include_dirs("freetype")
suppress_warnings("freetype")

cmake_minimum_required(VERSION 3.12)
include("third_party")

set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /nologo")

third_party("freetype" URL "https://download-mirror.savannah.gnu.org/releases/freetype/freetype-2.10.1.tar.xz"
    PATCHES
        "no_implicit_lib.patch"
        "version.patch"
        "zlib_debug.patch"
)
system_include_dirs("freetype")

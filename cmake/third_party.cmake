include(FetchContent)

function (third_party name repo ref)
    FetchContent_Declare("${name}"
        GIT_REPOSITORY "https://github.com/${repo}"
        GIT_TAG "${ref}"
    )
    FetchContent_GetProperties("${name}")
    if (NOT "${name}_POPULATED")
        FetchContent_Populate("${name}")
        add_subdirectory("${${name}_SOURCE_DIR}" "${${name}_BINARY_DIR}")
    endif()
endfunction ()


function (system_include_dirs target)
    get_target_property(inc "${target}" INTERFACE_INCLUDE_DIRECTORIES)
    set_target_properties("${target}" PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${inc}"
    )
endfunction ()


function (suppress_warnings target)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options("${target}" PRIVATE "-Wno-everything")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options("${target}" PRIVATE "-Wno-all -Wno-extra")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_compile_options("${target}" PRIVATE "/W0")
    endif ()
endfunction ()

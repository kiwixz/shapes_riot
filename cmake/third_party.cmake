include(FetchContent)

function (third_party name)
    cmake_parse_arguments(ARG "" "REPO;TAG;URL" "PATCHES" ${ARGN})

    if (ARG_REPO AND ARG_TAG AND NOT ARG_URL)
        FetchContent_Declare("${name}"
            GIT_REPOSITORY "https://github.com/${ARG_REPO}"
            GIT_TAG "${ARG_TAG}"
        )
    elseif (ARG_URL AND NOT ARG_REPO AND NOT ARG_TAG)
        FetchContent_Declare("${name}" URL "${ARG_URL}")
    else ()
        message(FATAL_ERROR "wrong arguments")
    endif()

    FetchContent_GetProperties("${name}")
    if (NOT "${name}_POPULATED")
        FetchContent_Populate("${name}")
        apply_patches("${${name}_SOURCE_DIR}" PATCHES ${ARG_PATCHES})
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


function (apply_patches dir)
    cmake_parse_arguments(ARG "" "" "PATCHES" ${ARGN})

    find_program(git_PATH git)
    foreach (patch ${ARG_PATCHES})
        get_filename_component(abs_patch "${patch}" ABSOLUTE)
        execute_process(COMMAND "${git_PATH}" apply --ignore-whitespace "${abs_patch}"
            WORKING_DIRECTORY "${dir}"
            RESULT_VARIABLE exit_code
        )
        if (exit_code)
            message(FATAL_ERROR "could not apply patch '${patch}'")
        endif ()
    endforeach ()
endfunction ()

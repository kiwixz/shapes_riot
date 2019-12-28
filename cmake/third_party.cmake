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

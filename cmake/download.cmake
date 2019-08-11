include(ExternalProject)

function (download_zip name url)
    ExternalProject_Add("${name}"
        SOURCE_DIR "${CURRENT_BINARY_DIR}/${name}"
        URL "${url}"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
    )

    set(${name}_DIR "${CURRENT_BINARY_DIR}/${name}" PARENT_SCOPE)
endfunction ()

function (download_github name repo ref)
    download_zip("${name}" "https://codeload.github.com/${repo}/zip/${ref}")
    set(${name}_DIR "${${name}_DIR}" PARENT_SCOPE)
endfunction ()

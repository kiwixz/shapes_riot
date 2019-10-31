function (doc_target)
    cmake_parse_arguments(ARG "" "" "DIRS" ${ARGN})
    if (UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "unparsed arguments")
    endif ()

    if (WIN32)
        set(DOC_CLANG_BACKEND "NO")
    else ()
        set(DOC_CLANG_BACKEND "YES")
    endif ()

    list(JOIN ARG_DIRS " " DOC_INPUT_DIRS)

    find_package("Doxygen" REQUIRED "dot")
    configure_file("${CMAKE_SOURCE_DIR}/cmake/doc_target.Doxyfile" "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target("doc"
        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_BINARY_DIR}/Doxyfile"
        COMMENT "Generating doc..."
        VERBATIM
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endfunction ()

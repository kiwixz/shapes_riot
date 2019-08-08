function (install_vcpkg vcpkg_ref)
    cmake_parse_arguments("ARG" "" "" "LIBS" ${ARGN})
    if (UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "unparsed arguments")
    endif ()

    find_package("Python3" COMPONENTS "Interpreter")
    execute_process(COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/cmake/install_vcpkg.py" "${vcpkg_ref}" ${ARG_LIBS})
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")
endfunction ()
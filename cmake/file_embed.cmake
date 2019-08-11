function (file_embed prefix name file)
    get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
    if (NOT "ASM_NASM" IN_LIST languages
        OR NOT "C" IN_LIST languages)  # also required by ASM_NASM to determine word size
        message(FATAL_ERROR "ASM_NASM or C not enabled")
    endif ()

    set(target "${prefix}_embed_${name}")

    configure_file("${CMAKE_SOURCE_DIR}/cmake/file_embed.asm" "${CMAKE_CURRENT_BINARY_DIR}/${target}/source.asm" @ONLY)
    add_library("${target}" STATIC "${CMAKE_CURRENT_BINARY_DIR}/${target}/source.asm")
    set_target_properties("${target}" PROPERTIES LINKER_LANGUAGE C)
    target_compile_options("${target}" PRIVATE "-DNAME=${name}" "-DFILE='${file}'")

    if (WIN32)
        target_compile_options("${target}" PRIVATE "-Xvc")
    endif ()

    configure_file("${CMAKE_SOURCE_DIR}/cmake/file_embed.h" "${CMAKE_CURRENT_BINARY_DIR}/${target}/include/embed/${name}.h" @ONLY)
    target_include_directories("${target}" BEFORE INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/${target}/include")

    set(${name}_EMBED "${target}" PARENT_SCOPE)
endfunction ()

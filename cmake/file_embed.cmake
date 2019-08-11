function (file_embed target prefix name file)
    get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
    if (NOT "ASM_NASM" IN_LIST languages
        OR NOT "C" IN_LIST languages)  # also required by ASM_NASM to determine word size
        message(FATAL_ERROR "ASM_NASM or C not enabled")
    endif ()

    set(embed_target "${prefix}_embed_${name}")

    configure_file("${CMAKE_SOURCE_DIR}/cmake/file_embed.asm" "${CMAKE_BINARY_DIR}/${embed_target}/source.asm" @ONLY)
    add_library("${embed_target}" STATIC "${CMAKE_BINARY_DIR}/${embed_target}/source.asm")
    set_target_properties("${embed_target}" PROPERTIES LINKER_LANGUAGE C)
    target_compile_options("${embed_target}" PRIVATE "-DNAME=${name}" "-DFILE='${file}'")

    if (WIN32)
        target_compile_options("${embed_target}" PRIVATE "-Xvc")
    endif ()

    configure_file("${CMAKE_SOURCE_DIR}/cmake/file_embed.h" "${CMAKE_BINARY_DIR}/${embed_target}/include/embed/${name}.h" @ONLY)
    target_include_directories("${embed_target}" BEFORE INTERFACE "${CMAKE_BINARY_DIR}/${embed_target}/include")

    target_link_libraries("${target}" "${embed_target}")
endfunction ()

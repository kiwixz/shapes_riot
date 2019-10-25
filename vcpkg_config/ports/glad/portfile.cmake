include(vcpkg_common_functions)

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO Dav1dde/glad
    REF v0.1.33
    SHA512 7a2d311308e0df147afa3e58b7a46fa0b0b456aaf4a9568a29a0cd7980b4645d2975be5c07ec35ef130ecb0d0f8d259e490fc4a028e60902867882f1ce7a8729
    HEAD_REF master
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DGLAD_PROFILE=core
        -DGLAD_API=gl=4.5

        -DGLAD_EXPORT=OFF
        -DGLAD_INSTALL=ON
        -DGLAD_REPRODUCIBLE=ON
)

vcpkg_install_cmake()
vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/glad)

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)
configure_file(${SOURCE_PATH}/LICENSE ${CURRENT_PACKAGES_DIR}/share/glad/copyright COPYONLY)

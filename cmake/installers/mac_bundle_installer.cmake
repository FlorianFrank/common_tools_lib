
function(build_mac_bundle)
    if (DEFINED PIL_STATIC)
        install(TARGETS common_tools_lib_static
                LIBRARY DESTINATION "lib"
                PUBLIC_HEADER DESTINATION "include")
    endif () # PIL_STATIC


    if (DEFINED PIL_SHARED)
        install(TARGETS common_tools_lib
                LIBRARY DESTINATION "lib"
                PUBLIC_HEADER DESTINATION "include")
    endif () # PIL_SHARED

    set(CPACK_BUNDLE_NAME "common_tools_lib")

    set(CPACK_PACKAGE_NAME "common_tools_lib")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${INSTALL_DIR})

    set(CPACK_PACKAGE_VERSION_MAJOR 1)
    set(CPACK_PACKAGE_VERSION_MINOR 0)
    set(CPACK_PACKAGE_VERSION_PATCH 0)

    set(CPACK_PACKAGE_VENDOR "University of Passau - Faculty for computer science and mathematics")
    set(CPACK_PACKAGE_CONTACT "Florian.Frank@uni-passau.de")

    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

    set(MACOSX_BUNDLE_NAME common_tools_lib)
    set(MACOSX_BUNDLE_VERSION 1.0.0)
    set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_LIST_DIR}/cmake/installers/mac_bundle/Info.plist")
    set(MACOSX_BUNDLE "TRUE")
    set(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_LIST_DIR}/cmake/installers/mac_bundle/logo.icns")

    include(CPack)
endfunction() # build_mac_bundle
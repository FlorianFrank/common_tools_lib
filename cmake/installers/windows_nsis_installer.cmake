function(build_nsis_installer)
    if (DEFINED PIL_STATIC)
        install(TARGETS common_tools_lib_static
                LIBRARY DESTINATION "lib"
                ARCHIVE "lib"
                RESOURCE "lib"
                PUBLIC_HEADER DESTINATION "include")
    endif () # PIL_STATIC


    if (DEFINED PIL_SHARED)
        install(TARGETS common_tools_lib
                LIBRARY DESTINATION "lib"
                ARCHIVE "lib"
                RESOURCE "lib"
                PUBLIC_HEADER DESTINATION "include")
    endif () # PIL_SHARED

    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/libs/windows/libgcc_s_seh-1.dll
                  ${CMAKE_CURRENT_SOURCE_DIR}/libs/windows/libstdc++-6.dll
                  ${CMAKE_CURRENT_SOURCE_DIR}/libs/windows/libwinpthread-1.dll
            DESTINATION include)

    # Custom target for packaging.
    if(USE_NSIS)
        set(CPACK_GENERATOR "NSIS")
    else(USE_NSIS)
        set(CPACK_GENERATOR "ZIP")
    endif(USE_NSIS)

    # CPACK Config
    set(CPACK_PACKAGE_NAME "common_tools_lib")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Platform Independent Lib for common programing tasks")

    set(CPACK_PACKAGE_VERSION_MAJOR 1)
    set(CPACK_PACKAGE_VERSION_MINOR 0)
    set(CPACK_PACKAGE_VERSION_PATCH 0)

    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${INSTALL_DIR})
    set(CPACK_PACKAGE_VENDOR "University of Passau - Faculty for computer science and mathematics")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_SOURCE_GENERATOR "TGZ")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "common_tools_lib_1.0.0")

    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_LIST_DIR}/cmake/installers/nsis_bundle/logo.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_LIST_DIR}/cmake/installers/nsis_bundle/logo.ico")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://florianfrank.github.io/")
    set(CPACK_NSIS_CONTACT "Florian.Frank@uni-passau.de")

    include(CPack)
endfunction() # build_nsis_installer





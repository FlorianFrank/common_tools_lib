
function(build_deb)
    set(INSTALL_DIR "/usr")
    if (DEFINED PIL_STATIC)
        install(TARGETS common_tools_lib_static
                LIBRARY DESTINATION "${INSTALL_DIR}/lib"
                PUBLIC_HEADER DESTINATION "${INSTALL_DIR}/include")
    endif() # PIL_STATIC


    if (DEFINED PIL_SHARED)
        install(TARGETS common_tools_lib
                LIBRARY DESTINATION "${INSTALL_DIR}/lib"
                PUBLIC_HEADER DESTINATION "${INSTALL_DIR}/include")
    endif() # PIL_STATIC

    # CPACK Config
    set(CPACK_PACKAGE_NAME "common_tools_lib")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Platform Independent Lib for common programing tasks")

    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${INSTALL_DIR})
    set(CPACK_DEB_COMPONENT_INSTALL YES)


    set(CPACK_PACKAGE_VERSION_MAJOR 1)
    set(CPACK_PACKAGE_VERSION_MINOR 0)
    set(CPACK_PACKAGE_VERSION_PATCH 0)

    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${ARCHITECTURE}")
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    set(CPACK_PACKAGE_VERSION_PATCH 0)

    set(CPACK_PACKAGE_VENDOR "University of Passau - Faculty for computer science and mathematics")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Florian Frank")
    set(CPACK_PACKAGE_CONTACT "Florian.Frank@uni-passau.de")

    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

    include(CPack)
endfunction() # build_debian
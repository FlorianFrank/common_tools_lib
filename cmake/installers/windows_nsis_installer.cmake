function(build_nsis_installer)
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

    # Custom target for packaging.
    if(USE_NSIS)
        set(CPACK_GENERATOR "NSIS")
    else(USE_NSIS)
        set(CPACK_GENERATOR "ZIP")
    endif(USE_NSIS)

    set(CPACK_PACKAGE_NAME "common_tools_lib")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_VERSION_PATCH "0")
    set(CPACK_PACKAGE_VENDOR "University of Passau - Faculty for computer science and mathematics")
    #set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${APP_NAME})
    #set(CPACK_IGNORE_FILES "\\.psd$;/CVS/;/\\.svn/;/\\.git/;\\.swp$;/CMakeLists.txt.user;\\.#;/#;\\.tar.gz$;/CMakeFiles/;CMakeCache.txt;\\.qm$;/build/;\\.diff$;.DS_Store'")
    set(CPACK_SOURCE_GENERATOR "TGZ")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "common_tools_lib_1.0.0")

    set(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_CURRENT_LIST_DIR}/cmake/installers/mac_bundle/logo.ico")
    #set(CPACK_NSIS_HELP_LINK ${APP_URL})
    #set(CPACK_NSIS_URL_INFO_ABOUT ${APP_URL})
    set(CPACK_NSIS_CONTACT "Florian.Frank@uni-passau.de")
endfunction() # build_nsis_installer





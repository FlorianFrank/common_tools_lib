cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-Wall -pedantic -Werror -g3 ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(PlatformIndependentLib C CXX)

if (DEFINED PIL_ALL)
    set(INSTALL_DIR "/usr")

    option(PIL_COMMUNICATION   "Enable UART and socket communication"       ON)
    option(PIL_THREADING       "Enable multithreading support"              ON)
    set(PIL_FILE_HANDLING      "Enable file handling support"               ON)
    set(PIL_LOGGING            "Enable logging support"                     ON)
    set(PIL_SHARED             "Build shared library"                       ON)
    set(PIL_STATIC             "Build static library"                       ON)
    set(PIL_UNIT_TESTING       "Enable Unit testing"                        ON)
    set(PIL_CSV                "Enable CSV file support"                    ON)
    set(PIL_CXX                "Enable C++ support"                         ON)
    set(PIL_EXCEPTION_HANDLING "Enable exception handling in C++ version"   ON)
    set(PIL_TIMING "Enable exception handling in C++ version"   ON)
endif () # PIL_ALL

if(PIL_CXX)
    add_definitions(-DPIL_CXX=1)
endif()

if(PIL_EXCEPTION_HANDLING)
    add_definitions(-DPIL_EXCEPTION_HANDLING=1)
endif()

if (DEFINED PIL_COMMUNICATION)
    if(DEFINED PIL_CXX)
    set(communication_headers_cxx
            "Communication/include/ctlib/Socket.hpp")

        set(communication_cxx "${CMAKE_CURRENT_SOURCE_DIR}/Communication/src/Socket.cpp")
    endif() # DEFINED CXX

    set(communication_headers "${communication_headers_cxx}"
    	"Communication/include/ctlib/Socket.h"
    	"Communication/include/ctlib/UARTInterface.h"
	    "Additional/ctlib/UARTDefines.h"
        "Additional/ctlib/SocketDefines.h")


    set(communication ${communication_cxx}
            "${CMAKE_CURRENT_SOURCE_DIR}/Communication/src/Socket.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/Communication/src/UARTInterface.c"
            "${communication_headers}")

    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Communication/include")
endif () # DEFINED COMMUNICATION

if (DEFINED PIL_TIMING)
        set(timing "Timing/include/ctlib/Timing.hpp" "Timing/src/Timing.cpp")
        include_directories("Timing/include")
endif()

if (DEFINED PIL_FILE_HANDLING)

    if(DEFINED PIL_CXX)
        set(file_handling_cpp
                "FileHandling/include/ctlib/FileHandler.hpp"
                "FileHandling/src/FileHandler.cpp")
    endif()

    set(file_handling_headers
             "FileHandling/include/ctlib/FileHandler.h"
            "Additional/ctlib/FileHandlingDefines.h")

    set(file_handling
            "${CMAKE_CURRENT_SOURCE_DIR}/FileHandling/src/FileHandler.c"
            "${file_handling_headers}"
            "${file_handling_cpp}")

    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/FileHandling/include")
endif() # DEFINED FILE_HANDLING

if (DEFINED PIL_LOGGING)
    add_definitions(-DPIL_LOGGING=1)
    set(logging_headers "Logging/include/ctlib/Logging.h")
    if(PIL_CXX)
        set(logging_cxx "${CMAKE_CURRENT_SOURCE_DIR}/Logging/src/Logging.cpp")
    endif()

    set(logging
        "${logging_cxx}"
    	"${CMAKE_CURRENT_SOURCE_DIR}/Logging/src/Logging.c" "${threading_headers_cxx}"
    	"${logging_headers}")

    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Logging/include")
endif () # DEFINED LOGGING

if(DEFINED PIL_THREADING)
    add_definitions(-DPIL_THREADING)

    set(threading_headers_cxx "Threading/include/ctlib/Threading.hpp")
    if(PIL_CXX)
        set(threading_cxx "${CMAKE_CURRENT_SOURCE_DIR}/Threading/src/Threading.cpp")
    endif()

    set(threading_headers
    		"Threading/include/ctlib/Threading.h"
            "Additional/ctlib/ThreadingDefines.h"
            "${threading_headers_cxx}")

    set(threading
    	"${CMAKE_CURRENT_SOURCE_DIR}/Threading/src/Threading.c"
    	"${threading_headers}"
        "${threading_cxx}")

    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Threading/include")

endif() # THREADING

if(DEFINED PIL_STRING) # TODO if CXX
    set(string "${CMAKE_CURRENT_SOURCE_DIR}/String/src/String.cpp"
               "String/include/ctlib/String.h")
endif() # STRING

if(DEFINED PIL_CSV)
    set(csv "${CMAKE_CURRENT_SOURCE_DIR}/Protocols/src/CSVHandler.c"
            "Protocols/include/ctlib/CSVHandler.h")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Protocols/include")
endif()

set(errorHandling_headers
        "ErrorHandling/include/ctlib/ExceptionHandler.h"
        "ErrorHandling/include/ctlib/ErrorHandler.h"
        "ErrorHandling/include/ctlib/ErrorCodeDefines.h")



set(errorHandling
        "${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/src/ErrorHandler.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/src/ExceptionHandler.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/src/Exception.cpp"
        ${errorHandling_headers})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/include")

set(additional_headers "Additional/ctlib/DataTypeDefines.h")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Additional")

set(source_files ${logging} ${communication} ${file_handling} ${errorHandling} ${additional_headers} ${unitTestFiles} ${threading} ${string} ${csv} ${timing})

if (DEFINED PIL_SHARED)
    add_library(common_tools_lib SHARED ${source_files})
    if(PIL_STATIC_LIBGCC)
        target_link_options(common_tools_lib PUBLIC -static -static-libgcc -static-libstdc++)
    endif()


    if(WIN32)
        target_link_libraries(common_tools_lib  PRIVATE ws2_32 wsock32)
    else()
        target_link_libraries(common_tools_lib  PRIVATE pthread)
    endif()
    set_target_properties(common_tools_lib PROPERTIES PUBLIC_HEADER
    	"${errorHandling_headers};${communication_headers};${file_handling_headers};${logging_headers};${threading_headers};${additional_headers}")

	INSTALL(TARGETS common_tools_lib
	        LIBRARY DESTINATION "${INSTALL_DIR}/lib"
	        PUBLIC_HEADER DESTINATION "${INSTALL_DIR}/include"
)
endif () # DEFINED SHARED

if (DEFINED PIL_STATIC)
    add_library(common_tools_lib_static STATIC ${source_files})

    if(PIL_STATIC_LIBGCC)
       target_link_options(common_tools_lib_static PUBLIC -static -static-libgcc -static-libstdc++)
    endif()

    if(WIN32)
        target_link_libraries(common_tools_lib_static  PRIVATE ws2_32 wsock32)
    else()
        target_link_libraries(common_tools_lib_static  PRIVATE pthread)
    endif()
#	install(TARGETS PIL_Static DESTINATION "${CMAKE_")

endif () # DEFINED STATIC

if (DEFINED ENV{GITHUB_ACTIONS})
    if (WIN32)

        link_directories("googletest/build/lib")
        include_directories("googletest/googletest/include/")
    endif ()
endif ()

if (PIL_UNIT_TESTING)
    if(DEFINED APPLE)
        include_directories("/usr/local/include")
        link_directories("/usr/local/lib")
    endif() # APPLE

    enable_testing()
    include(GoogleTest)

    set(SOCKET_UNIT_TEST_FILES "UnitTesting/SocketUnitTest.cpp")
    add_executable(SocketUnitTest ${SOCKET_UNIT_TEST_FILES})
    target_link_libraries(SocketUnitTest gtest_main gtest pthread common_tools_lib)
    gtest_discover_tests("SocketUnitTest")

    set(EXCEPTION_UNIT_TEST_FILES "UnitTesting/ExceptionUnitTest.cpp")
    add_executable(ExceptionUnitTest ${EXCEPTION_UNIT_TEST_FILES})
    target_link_libraries(ExceptionUnitTest gtest_main gtest pthread common_tools_lib)
    gtest_discover_tests("ExceptionUnitTest")

    set(FILE_HANDLING_UNIT_TEST_FILES "UnitTesting/ExceptionUnitTest.cpp")
    add_executable(FileHandlingUnitTest ${FILE_HANDLING_UNIT_TEST_FILES})
    target_link_libraries(FileHandlingUnitTest gtest_main gtest pthread common_tools_lib)
    gtest_discover_tests("FileHandlingUnitTest")

endif () # PIL_UNIT_TESTING

option(BUILD_DOC "Build documentation" OFF)

# check if Doxygen is installed
if(DEFINED PIL_BUILD_DOCUMENTATION)
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
endif() # DEFINED BUILD_DOCUMENTATION


    ### CPack Config ###
SET(CPACK_PACKAGE_NAME "common_tools_lib")
SET(CPACK_PACKAGE_VERSION_MAJOR 1)
SET(CPACK_PACKAGE_VERSION_MINOR 0)
SET(CPACK_PACKAGE_VERSION_PATCH 0)
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "University of Passau")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Platform Independent Lib for common programing tasks")
SET(CPACK_PACKAGE_VENDOR "Faculty for computer science and mathematics")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${ARCHITECTURE}")
SET(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
SET(CPACK_PACKAGE_VERSION_PATCH 0)

INCLUDE(CPack)

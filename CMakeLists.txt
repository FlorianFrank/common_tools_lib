cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-Wall -pedantic -Werror -g3 ${CMAKE_C_FLAGS}")

project(PlatformIndependentLib C CXX)

if (DEFINED ALL)
    set(INSTALL_DIR "/usr")
    set(COMMUNICATION 1)
    set(THREADING 1)
    set(FILE_HANDLING 1)
    set(LOGGING 1)
    set(SHARED 1)
    set(SHARED 1)
    set(STATIC 1)
    set(CSV 1)
    set(CXX 1)
    add_definitions(-DCXX)
#    set(UNIT_TESTING 1)
#    set(STRING 0)
endif () # DEFINED ALL

if (DEFINED COMMUNICATION)
	message("HALLO")
    if(DEFINED CXX)
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

if (DEFINED FILE_HANDLING)

    set(file_handling_headers
             "FileHandling/include/ctlib/FileHandler.h"
            "Additional/ctlib/FileHandlingDefines.h")

    set(file_handling
            "${CMAKE_CURRENT_SOURCE_DIR}/FileHandling/src/FileHandler.c"
            "${file_handling_headers}")

    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/FileHandling/include")
endif() # DEFINED FILE_HANDLING

if (DEFINED LOGGING)

    set(logging_headers "Logging/include/ctlib/Logging.h")

    set(logging 
    	"${CMAKE_CURRENT_SOURCE_DIR}/Logging/src/Logging.cpp" "${threading_headers_cxx}"
    	"${logging_headers}")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Logging/include")
endif () # DEFINED LOGGING

if(DEFINED THREADING)
    add_definitions(-DPIL_THREADING)

    set(threading_headers_cxx "Threading/include/ctlib/Threading.hpp")
    set(threading_cxx "${CMAKE_CURRENT_SOURCE_DIR}/Threading/src/Threading.cpp")

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

if(DEFINED STRING) # TODO if CXX
    set(string "${CMAKE_CURRENT_SOURCE_DIR}/String/src/String.cpp"
               "String/include/ctlib/String.h")
endif() # STRING

if(DEFINED CSV)
    set(csv "${CMAKE_CURRENT_SOURCE_DIR}/Protocols/src/CSVHandler.c"
            "Protocols/include/ctlib/CSVHandler.h")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Protocols/include")
endif()

set(errorHandling_headers
        "ErrorHandling/include/ctlib/ErrorHandler.h"
        "ErrorHandling/include/ctlib/ErrorCodeDefines.h")



set(errorHandling
        "${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/src/ErrorHandler.c"
        ${errorHandling_headers})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/include")

set(additional_headers "Additional/ctlib/DataTypeDefines.h")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Additional")

set(source_files ${logging} ${communication} ${file_handling} ${errorHandling} ${additional_headers} ${unitTestFiles} ${threading} ${string} ${csv})

if (DEFINED SHARED)
    add_library(common_tools_lib SHARED ${source_files})
    set_target_properties(common_tools_lib PROPERTIES PUBLIC_HEADER
    	"${errorHandling_headers};${communication_headers};${file_handling_headers};${logging_headers};${threading_headers};${additional_headers}")

	INSTALL(TARGETS common_tools_lib
	        LIBRARY DESTINATION "${INSTALL_DIR}/lib"
	        PUBLIC_HEADER DESTINATION "${INSTALL_DIR}/include"
)
endif () # DEFINED SHARED

if (DEFINED STATIC)
    add_library(common_tools_lib_static STATIC ${source_files})
#	install(TARGETS PIL_Static DESTINATION "${CMAKE_")

endif () # DEFINED STATIC

if (UNIT_TESTING)
    set(socketUnitTest "${CMAKE_CURRENT_SOURCE_DIR}/UnitTesting/SocketUnitTest.cpp")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/UnitTesting")
    add_executable(SocketUnitTest ${socketUnitTest})
    target_link_libraries(SocketUnitTest common_tools_lib gtest pthread)
endif () # UNIT_TESTING

add_executable(SOCKET_UNIT_TEST "UnitTesting/SocketUnitTest.cpp")
target_link_libraries(SOCKET_UNIT_TEST gtest_main gtest pthread common_tools_lib)

enable_testing()
include(GoogleTest)
gtest_discover_tests("SOCKET_UNIT_TEST")

option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
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

add_executable(CSV_Test UnitTesting/CSV_Test.c)
target_link_libraries(CSV_Test common_tools_lib pthread)

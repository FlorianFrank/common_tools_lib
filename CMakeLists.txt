cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-Wall -pedantic -Werror -g3 ${CMAKE_C_FLAGS}")

project(PlatformIndependentLib C CXX)

if (DEFINED ALL)
    set(INSTALL_DIR "/usr/local")
    set(COMMUNICATION 1)
    set(THREADING 1)
    set(FILE_HANDLING 1)
    set(LOGGING 1)
    set(SHARED 1)
    set(SHARED 1)
    set(STATIC 1)
#    set(UNIT_TESTING 1)
#    set(STRING 0)
endif () # DEFINED ALL

if (DEFINED COMMUNICATION)

    set(communication_headers             
    	"${CMAKE_CURRENT_SOURCE_DIR}/Communication/include/Socket.h"
    	"${CMAKE_CURRENT_SOURCE_DIR}/Communication/include/UARTInterface.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Additional/UARTDefines.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Additional/SocketDefines.h")

    set(communication
            "${CMAKE_CURRENT_SOURCE_DIR}/Communication/src/Socket.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/Communication/src/UARTInterface.c"
            "${communication_headers}")
            
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Communication/include")
    
endif () # DEFINED COMMUNICATION

if (DEFINED FILE_HANDLING)

    set(file_handling_headers 
             "${CMAKE_CURRENT_SOURCE_DIR}/FileHandling/include/FileHandler.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/Additional/FileHandlingDefines.h")

    set(file_handling
            "${CMAKE_CURRENT_SOURCE_DIR}/FileHandling/src/FileHandler.c"
            "${file_handling_headers}")
            
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/FileHandling/include")
endif() # DEFINED FILE_HANDLING

if (DEFINED LOGGING)

    set(logging_headers "${CMAKE_CURRENT_SOURCE_DIR}/Logging/include/Logging.h")

    set(logging 
    	"${CMAKE_CURRENT_SOURCE_DIR}/Logging/src/Logging.cpp"
    	"${logging_headers}")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Logging/include")
endif () # DEFINED LOGGING

if(DEFINED THREADING)

    set(threading_headers                   
    		"${CMAKE_CURRENT_SOURCE_DIR}/Threading/include/Threading.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/Additional/ThreadingDefines.h")

    set(threading 
    	"${CMAKE_CURRENT_SOURCE_DIR}/Threading/src/Threading.c"
    	"${threading_headers}")
    	
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Threading/include")

endif() # THREADING

if(DEFINED STRING) # TODO if CXX
    set(string "${CMAKE_CURRENT_SOURCE_DIR}/String/src/String.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/String/include/String.h")
endif() # STRING


set(errorHandling_headers 
        "${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/include/ErrorHandler.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/include/ErrorCodeDefines.h")

set(errorHandling
        "${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/src/ErrorHandler.c"
        ${errorHandling_headers})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/include")

set(additional_headers "${CMAKE_CURRENT_SOURCE_DIR}/Additional/DataTypeDefines.h")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Additional")

set(source_files ${logging} ${communication} ${file_handling} ${errorHandling} ${additional_headers} ${unitTestFiles} ${threading} ${string})

if (DEFINED SHARED)
    add_library(PIL SHARED ${source_files})
    set_target_properties(PIL PROPERTIES PUBLIC_HEADER 
    	"${errorHandling_headers};${communication_headers};${file_handling_headers};${logging_headers};${threading_headers};${additional_headers}")

	INSTALL(TARGETS PIL 
	        LIBRARY DESTINATION "${INSTALL_DIR}/lib"
	        PUBLIC_HEADER DESTINATION "${INSTALL_DIR}/include"
)
endif () # DEFINED SHARED

if (DEFINED STATIC)
    add_library(PIL_Static STATIC ${source_files})
#	install(TARGETS PIL_Static DESTINATION "${CMAKE_")

endif () # DEFINED STATIC

if (UNIT_TESTING)
    set(socketUnitTest "${CMAKE_CURRENT_SOURCE_DIR}/UnitTesting/SocketUnitTest.cpp")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/UnitTesting")
    add_executable(SocketUnitTest ${socketUnitTest})
    target_link_libraries(SocketUnitTest PIL gtest pthread)
endif () # UNIT_TESTING

#add_executable(TEST2 UnitTesting/SocketTest.c)
#target_link_libraries(TEST2 PIL gtest pthread)


### CPack Config ###
SET(CPACK_PACKAGE_NAME "PILLib")
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

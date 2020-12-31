cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "-Wall -pedantic -Werror -g3 ${CMAKE_C_FLAGS}")

project(PlatformIndependentLib C CXX)

if (DEFINED ALL)
    set(COMMUNICATION 1)
    set(LOGGING 1)
    set(SHARED 1)
    set(SHARED 1)
    set(STATIC 1)
    set(UNIT_TESTING 1)
endif () # DEFINED ALL

if (DEFINED COMMUNICATION)
    set(communication
            "${CMAKE_CURRENT_SOURCE_DIR}/Communication/src/Socket.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/Communication/include/Socket.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/Additional/SocketDefines.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/Communication/src/UARTInterface.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/Additional/UARTDefines.h")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Communication/include")
endif () # DEFINED COMMUNICATION

if (DEFINED LOGGING)
    set(logging "${CMAKE_CURRENT_SOURCE_DIR}/Logging/src/Logging.cpp")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Logging/include")
endif () # DEFINED LOGGING

if(DEFINED THREADING)
    set(threading "${CMAKE_CURRENT_SOURCE_DIR}/Threading/src/Threading.c"
                  "${CMAKE_CURRENT_SOURCE_DIR}/Threading/include/Threading.h")
endif() # THREADING

set(errorHandling
        "${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/src/ErrorHandler.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/include/ErrorHandler.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/include/ErrorCodeDefines.h")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/include")

set(additionalFiles "${CMAKE_CURRENT_SOURCE_DIR}/Additional/DataTypeDefines.h")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Additional")

set(source_files ${logging} ${communication} ${errorHandling} ${additionalFiles} ${unitTestFiles})

if (DEFINED SHARED)
    add_library(PIL SHARED ${source_files})
endif () # DEFINED SHARED

if (DEFINED STATIC)
    add_library(PIL_Static STATIC ${source_files})
endif () # DEFINED STATIC


if (UNIT_TESTING)
    set(socketUnitTest "${CMAKE_CURRENT_SOURCE_DIR}/UnitTesting/SocketUnitTest.cpp")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/UnitTesting")
    add_executable(SocketUnitTest ${socketUnitTest})
    target_link_libraries(SocketUnitTest PIL gtest pthread)
endif () # UNIT_TESTING
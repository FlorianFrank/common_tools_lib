cmake_minimum_required(VERSION 3.0)

set(errorHandling_headers
        "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/ExceptionHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/ErrorHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/ErrorCodeDefines.h")

set(errorHandling
        "${CMAKE_CURRENT_LIST_DIR}/src/ErrorHandler.c"
        "${CMAKE_CURRENT_LIST_DIR}/src/ExceptionHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/Exception.cpp"
        ${errorHandling_headers})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ErrorHandling/include")
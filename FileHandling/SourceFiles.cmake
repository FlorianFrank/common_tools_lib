cmake_minimum_required(VERSION 3.0)

if (DEFINED PIL_CXX)
    set(file_handling_cpp
            "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/FileHandler.hpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/FileHandler.cpp")
endif () # PIL_CXX

set(file_handling_headers
        "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/FileHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/../Additional/ctlib/FileHandlingDefines.h")

set(file_handling
        "${CMAKE_CURRENT_LIST_DIR}/src/FileHandler.c"
        "${file_handling_headers}"
        "${file_handling_cpp}")

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
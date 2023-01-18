cmake_minimum_required(VERSION 3.0)

if (DEFINED PIL_CXX)
    set(communication_headers_cxx
            "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/Socket.hpp")

    set(communication_cxx "${CMAKE_CURRENT_LIST_DIR}/src/Socket.cpp")
endif () # DEFINED CXX

set(communication_headers "${communication_headers_cxx}"
        "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/Socket.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/UARTInterface.h"
        "${CMAKE_CURRENT_LIST_DIR}/../Additional/ctlib/UARTDefines.h"
        "${CMAKE_CURRENT_LIST_DIR}/../Additional/ctlib/SocketDefines.h")


set(communication ${communication_cxx}
        "${CMAKE_CURRENT_LIST_DIR}/src/Socket.c"
        "${CMAKE_CURRENT_LIST_DIR}/src/UARTInterface.c"
        "${communication_headers}")

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
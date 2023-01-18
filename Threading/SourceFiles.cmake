cmake_minimum_required(VERSION 3.0)

add_definitions(-DPIL_THREADING)

set(threading_headers_cxx "Threading/include/ctlib/Threading.hpp")
if(PIL_CXX)
    set(threading_headers_cxx ${threading_headers_cxx}
            "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/ThreadArg.hpp")
    set(threading_cxx "${CMAKE_CURRENT_LIST_DIR}/src/Threading.cpp")
endif() # PIL_CXX

set(threading_headers
        "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/Threading.h"
        "${CMAKE_CURRENT_LIST_DIR}/../Additional/ctlib/ThreadingDefines.h"
        "${threading_headers_cxx}")

set(threading
        "${CMAKE_CURRENT_LIST_DIR}/src/Threading.c"
        "${threading_headers}")

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
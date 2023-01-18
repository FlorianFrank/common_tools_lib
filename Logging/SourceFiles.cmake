cmake_minimum_required(VERSION 3.0)

add_definitions(-DPIL_LOGGING=1)
set(logging_headers "${CMAKE_CURRENT_LIST_DIR}/include/ctlib/Logging.h")
if(PIL_CXX)
    set(logging_cxx "${CMAKE_CURRENT_LIST_DIR}/src/Logging.cpp")
endif() # PIL_CXX

set(logging
        "${logging_cxx}"
        "${CMAKE_CURRENT_LIST_DIR}/src/Logging.c"
        "${threading_headers_cxx}"
        "${logging_headers}")

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
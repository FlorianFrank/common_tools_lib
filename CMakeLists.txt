cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-Wall -pedantic -Werror -g3 ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(common_tools_lib C CXX)

if (DEFINED PIL_ALL)
    option(PIL_SHARED               "Build shared library"                       ON)
    option(PIL_STATIC               "Build static library"                       ON)

    option(PIL_CXX                  "Enable C++ support"                         ON)
    option(PIL_EXCEPTION_HANDLING   "Enable exception handling in C++ version"   ON)

    option(PIL_COMMUNICATION        "Enable UART and socket communication"       ON)
    option(PIL_THREADING            "Enable multithreading support"              ON)
    option(PIL_FILE_HANDLING        "Enable file handling support"               ON)
    option(PIL_LOGGING              "Enable logging support"                     ON)
    option(PIL_TIMING               "Enable exception handling in C++ version"   ON)
    option(PIL_CSV                  "Enable CSV file support"                    ON)

    option(PIL_BUILD_DOCUMENTATION "Build doxygen code documentation"            OFF)

    if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
        option(PIL_UNIT_TESTING     "Enable Unit testing"                        ON)
    endif()
endif () # PIL_ALL

if(PIL_CXX)
    add_definitions(-DPIL_CXX=1)
endif() # PIL_CXX

if(PIL_EXCEPTION_HANDLING)
    add_definitions(-DPIL_EXCEPTION_HANDLING=1)
endif()

if (PIL_COMMUNICATION)
    include(Communication/SourceFiles.cmake)
endif () # DEFINED COMMUNICATION

if (PIL_TIMING)
    include(Timing/SourceFiles.cmake)
endif()

if (PIL_FILE_HANDLING)
    include(FileHandling/SourceFiles.cmake)
endif() # DEFINED FILE_HANDLING

if (PIL_LOGGING)
    include(Logging/SourceFiles.cmake)
endif () # DEFINED LOGGING

if(PIL_THREADING)
    include("Threading/SourceFiles.cmake")
endif() # THREADING

if(PIL_STRING)
    set(string "${CMAKE_CURRENT_SOURCE_DIR}/String/src/String.cpp"
            "String/include/ctlib/String.h")
endif() # STRING

if(PIL_CSV)
    set(csv "${CMAKE_CURRENT_SOURCE_DIR}/Protocols/src/CSVHandler.c"
            "Protocols/include/ctlib/CSVHandler.h")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Protocols/include")
endif()

include("ErrorHandling/SourceFiles.cmake")

set(additional_headers "Additional/ctlib/DataTypeDefines.h")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Additional")

set(source_files ${logging} ${communication} ${file_handling} ${errorHandling} ${additional_headers} ${unitTestFiles} ${threading} ${string} ${csv} ${timing_cxx})

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
            "${errorHandling_headers};${communication_headers};${file_handling_headers};${logging_headers};${threading_headers};${additional_headers};${timing_cxx_headers}")
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
endif () # DEFINED STATIC

if (PIL_UNIT_TESTING)
    add_subdirectory(googletest)
    enable_testing()
    include(GoogleTest)

    include(UnitTesting/build_unittest.cmake)
endif () # PIL_UNIT_TESTING


if(PIL_BUILD_DOCUMENTATION)
    include(cmake/build_documentation.cmake)
    pil_build_documentation()
endif() # DEFINED BUILD_DOCUMENTATION

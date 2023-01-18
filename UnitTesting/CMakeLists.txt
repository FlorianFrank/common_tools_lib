cmake_minimum_required(VERSION 3.0)

set(socket_unit_test_files "${CMAKE_CURRENT_LIST_DIR}/SocketUnitTest.cpp")
add_executable(SocketUnitTest ${socket_unit_test_files})
target_link_libraries(SocketUnitTest gtest_main gtest pthread common_tools_lib)
gtest_discover_tests("SocketUnitTest")

set(exception_unit_test_files "${CMAKE_CURRENT_LIST_DIR}/ExceptionUnitTest.cpp")
add_executable(ExceptionUnitTest ${exception_unit_test_files})
target_link_libraries(ExceptionUnitTest gtest_main gtest pthread common_tools_lib)
gtest_discover_tests("ExceptionUnitTest")

set(file_handling_unit_test_files "${CMAKE_CURRENT_LIST_DIR}/ExceptionUnitTest.cpp")
add_executable(FileHandlingUnitTest ${file_handling_unit_test_files})
target_link_libraries(FileHandlingUnitTest gtest_main gtest pthread common_tools_lib)
gtest_discover_tests("FileHandlingUnitTest")

set(threading_unit_test_files "${CMAKE_CURRENT_LIST_DIR}/ThreadingUnitTest.cpp")
add_executable(ThreadingUnitTest ${threading_unit_test_files})
target_link_libraries(ThreadingUnitTest gtest_main gtest pthread common_tools_lib)
gtest_discover_tests("ThreadingUnitTest")
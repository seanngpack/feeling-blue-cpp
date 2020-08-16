cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp ${PROJECT_SOURCE_DIR}/test/*.mm)

set(TEST_MAIN unit_tests)

include_directories(
        ${PROJECT_SOURCE_DIR}/macOS/include/wrapper
        ${PROJECT_SOURCE_DIR}/macOS/src
        ${PROJECT_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
        ${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

add_executable(${TEST_MAIN} ${TEST_SRC_FILES})
target_link_libraries(${TEST_MAIN} gtest gtest_main
        "-framework Foundation"
        "-framework CoreBluetooth")

add_test(${TEST_MAIN} ${TEST_MAIN})

cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    message(windows support is currently being added for windows)
endif (WIN32)

if(APPLE)
    add_subdirectory(macOS)
endif()
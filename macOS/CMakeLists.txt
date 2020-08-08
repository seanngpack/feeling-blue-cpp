cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

set(macOS_SRCS ./src/blue.cpp ./src/stuff/stuff.cpp)
set(macOS_PUBLIC_HEADERS
        ./include
        ./include/stuff)

include_directories(./include/stuff) # need for nesting soon

add_library(feeling-blue SHARED ${macOS_SRCS})


target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

install(TARGETS ${PROJECT_NAME}
        EXPORT feeling-blueConfig
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${macOS_PUBLIC_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(EXPORT feeling-blueConfig DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/feeling-blue/cmake)

export(TARGETS ${PROJECT_NAME} FILE feeling-blueConfig.cmake)
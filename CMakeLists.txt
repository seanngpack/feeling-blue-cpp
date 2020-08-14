cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

project(feeling-blue
        VERSION 1.0
        DESCRIPTION "mylib description")

include(GNUInstallDirs)

set(SOURCE_FILES macOS/src/main.cpp)

# set the public headers you want the client to user!
set(macOS_PUBLIC_HEADERS
        ./include/feeling-blue.h
        )

set(macOS_PRIVATE_HEADERS
        ./include/central.h
        ./include/peripheral.h
        ./include/service.h
        ./include/characteristic.h
        )

add_subdirectory(macOS)


message(CMAKE_INSTALL_DIR = ${CMAKE_INSTALL_LIBDIR})
message(CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR})
message(CMAKE_INSTALL_DATAROOTDIR = ${CMAKE_INSTALL_DATAROOTDIR})
message(CMAKE_INSTALL_INCLUDEDIR = ${CMAKE_INSTALL_INCLUDEDIR})
message(where_is_this = ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
message(CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR})


target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)


set_target_properties(${PROJECT_NAME}
        PROPERTIES
        PUBLIC_HEADER "${macOS_PUBLIC_HEADERS}"
        PRIVATE_HEADER "${macOS_PRIVATE_HEADERS}"
        )

install(TARGETS ${PROJECT_NAME}
        EXPORT feeling-blueConfig
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(EXPORT feeling-blueConfig DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/feeling-blue/cmake)

export(TARGETS ${PROJECT_NAME} FILE feeling-blueConfig.cmake)
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

project(feeling-blue
        VERSION 1.02
        DESCRIPTION "mylib description")

include(GNUInstallDirs)

set(SOURCE_FILES macOS/src/main.cpp)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# --------------------------------------------------------------------------------
#                          Build options
# --------------------------------------------------------------------------------

option(BUILD_DOCUMENTATION "Create the API documentation" OFF)
option(BUILD_TESTS "Build the tests" OFF)
option(VERBOSE_MODE "NSLog what your bluetooth device is doing" ON)


# --------------------------------------------------------------------------------
#                          Public headers API
# --------------------------------------------------------------------------------
set(macOS_PUBLIC_HEADERS
        ./include/feeling-blue.h
        ./include/central.h
        ./include/peripheral.h
        ./include/service.h
        ./include/characteristic.h
        )

set(macOS_PRIVATE_HEADERS
        ./include/detail/conversions.h
        )

# --------------------------------------------------------------------------------
#                          Enable Testing
# --------------------------------------------------------------------------------

include(CTest)
enable_testing()


# --------------------------------------------------------------------------------
#                          Subdirectories
# --------------------------------------------------------------------------------

add_subdirectory(macOS)

if (BUILD_TESTS)
    message("enabled building tests")
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/googletest-release-1.10.0)
    add_subdirectory(test)
endif()


# --------------------------------------------------------------------------------
#                          Add directories
# --------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        PUBLIC_HEADER "${macOS_PUBLIC_HEADERS}"
        PRIVATE_HEADER "${macOS_PRIVATE_HEADERS}"
        )

# --------------------------------------------------------------------------------
#                          Doxygen Subdirectory
# --------------------------------------------------------------------------------

add_subdirectory(docs)

# --------------------------------------------------------------------------------
#                          Install library
# --------------------------------------------------------------------------------
install(TARGETS ${PROJECT_NAME}
        EXPORT feeling-blueConfig
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(EXPORT feeling-blueConfig DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/feeling-blue/cmake)

export(TARGETS ${PROJECT_NAME} FILE feeling-blueConfig.cmake)
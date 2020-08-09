cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)


# set the source files!
set(macOS_SRCS
        src/wrapper/bluetooth.mm
        src/model/central_event_handler.cpp include/model/peripheral_mac.h
        src/model/peripheral_mac.cpp include/model/central_mac.h src/model/central_mac.cpp ../include/feeling-blue.h)

# set the public headers you want the client to user!
set(macOS_PUBLIC_HEADERS
        ../include/feeling-blue.h
        )

# set the include directories so source files can find them
include_directories(
        include/wrapper
        include/model)

add_library(feeling-blue SHARED ${macOS_SRCS})

#set(CMAKE_EXE_LINKER_FLAGS "-framework CoreBluetooth")
#set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation")
target_link_libraries(feeling-blue PUBLIC
        "-framework Foundation"
        "-framework CoreBluetooth")
#set_target_properties(feeling-blue PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

install(TARGETS ${PROJECT_NAME}
        EXPORT feeling-blueConfig
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${macOS_PUBLIC_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(EXPORT feeling-blueConfig DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/feeling-blue/cmake)

export(TARGETS ${PROJECT_NAME} FILE feeling-blueConfig.cmake)

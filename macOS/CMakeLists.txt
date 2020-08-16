cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)


# set the source files!
set(macOS_SRCS
        src/wrapper/bluetooth.mm
        src/model/peripheral_impl.cpp src/model/central_impl.cpp
        ../include/feeling-blue.h
        ../include/peripheral.h
        ../include/central.h
        ../include/service.h
        src/model/service_impl.cpp ../include/characteristic.h src/model/characteristic_impl.cpp
        )


include_directories(
        include/wrapper
        include/model)

add_library(feeling-blue SHARED ${macOS_SRCS})

target_link_libraries(feeling-blue PUBLIC
        "-framework Foundation"
        "-framework CoreBluetooth")

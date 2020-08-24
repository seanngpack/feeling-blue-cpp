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
        ../include/detail/conversions.h
        src/model/service_impl.cpp
        ../include/characteristic.h src/model/characteristic_impl.cpp
        )


include_directories(
        ../include/detail
        include/wrapper
        include/model)

add_library(feeling-blue SHARED ${macOS_SRCS})

# debug mode or nah
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    message("we're in debug mode")
    target_compile_definitions(feeling-blue PRIVATE DEBUG_MODE=1)
else()
    message("we're not in debug mode")
    target_compile_definitions(feeling-blue PRIVATE DEBUG_MODE=0)
endif()

# verbose mode or nah
if(VERBOSE_MODE)
    message("we're in verbose mode")
    target_compile_definitions(feeling-blue PRIVATE VERBOSE_MODE=1)
else()
    message("we're not in verbose mode")
    target_compile_definitions(feeling-blue PRIVATE VERBOSE_MODE=0)
endif()


target_link_libraries(feeling-blue PUBLIC
        "-framework Foundation"
        "-framework CoreBluetooth")

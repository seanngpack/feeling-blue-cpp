cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)


# set the source files!
set(macOS_SRCS
        wrapper/wrapper.h
        wrapper/bluetooth.mm
        model/peripheral_impl.cpp model/central_impl.cpp
        model/characteristic_impl.cpp
        model/service_impl.cpp
        ${PROJECT_SOURCE_DIR}/include/feeling-blue.h
        ${PROJECT_SOURCE_DIR}/include/peripheral.h
        ${PROJECT_SOURCE_DIR}/include/central.h
        ${PROJECT_SOURCE_DIR}/include/service.h
        ${PROJECT_SOURCE_DIR}/include/detail/conversions.h
        ${PROJECT_SOURCE_DIR}/include/characteristic.h
        )


include_directories(
        ${PROJECT_SOURCE_DIR}/external
        ${PROJECT_SOURCE_DIR}/include/detail
        wrapper
        wrapper
)

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

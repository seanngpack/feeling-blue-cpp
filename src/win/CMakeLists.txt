# set the source files!
set(win_SRCS
#        model/characteristic_impl.cpp
        ${PROJECT_SOURCE_DIR}/include/feeling-blue.h
        ${PROJECT_SOURCE_DIR}/include/peripheral.h
        ${PROJECT_SOURCE_DIR}/include/central.h
        ${PROJECT_SOURCE_DIR}/include/service.h
        ${PROJECT_SOURCE_DIR}/include/detail/conversions.h
        ${PROJECT_SOURCE_DIR}/include/characteristic.h
        ${PROJECT_SOURCE_DIR}/include/pch.h
        pch.cpp
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/external
        ${PROJECT_SOURCE_DIR}/include/detail
)

add_library(feeling-blue SHARED ${win_SRCS})
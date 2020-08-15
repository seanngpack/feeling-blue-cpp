cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)

# --------------------------------------------------------------------------------
#                          DOXYGEN
# --------------------------------------------------------------------------------
if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)

    get_target_property(FEELING_BLUE_PUBLIC_HEADER_DIR feeling-blue INTERFACE_INCLUDE_DIRECTORIES)

    file(GLOB_RECURSE FEELING_BLUE_PUBLIC_HEADERS
            ${FEELING_BLUE_PUBLIC_HEADER_DIR}/*.h)

    set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/macOS)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    #Replace variables inside @@ with the current values
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) #Doxygen won't create this for us

    add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
            DEPENDS ${FEELING_BLUE_PUBLIC_HEADERS}
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
            MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
            COMMENT "Generating docs"
            )

    add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})
endif()

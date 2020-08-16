cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)



if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    find_package(Sphinx REQUIRED)

    # --------------------------------------------------------------------------------
    #                          DOXYGEN
    # --------------------------------------------------------------------------------

    get_target_property(FEELING_BLUE_PUBLIC_HEADER_DIR feeling-blue INTERFACE_INCLUDE_DIRECTORIES)

    file(GLOB_RECURSE FEELING_BLUE_PUBLIC_HEADERS
            ${FEELING_BLUE_PUBLIC_HEADER_DIR}/*.h)

    set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/macOS) # NOTE: might change this in future
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    #Replace variables inside @@ with the current values
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) #Doxygen won't create this for us
    message(Created doxygen output dir=${DOXYGEN_OUTPUT_DIR})

    add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
            DEPENDS ${FEELING_BLUE_PUBLIC_HEADERS}
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
            MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
            COMMENT "Generating docs"
            VERBATIM)

    add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})


    # --------------------------------------------------------------------------------
    #                          SPHINX
    # --------------------------------------------------------------------------------

    set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
    set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
    set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

    add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
            COMMAND
            ${SPHINX_EXECUTABLE} -b html
            # Tell Breathe where to find the Doxygen output
            -Dbreathe_projects.feeling-blue=${DOXYGEN_OUTPUT_DIR}/xml
            ${SPHINX_SOURCE} ${SPHINX_BUILD}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS
            # Other docs files you want to track should go here (or in some variable)
                ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
                ${DOXYGEN_INDEX_FILE}
            MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
            COMMENT "Generating documentation with Sphinx")

    add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})
endif()
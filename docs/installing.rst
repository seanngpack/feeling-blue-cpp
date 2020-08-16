************
Installation
************

Download and build:

.. code:: console

    $ git clone https://github.com/seanngpack/feeling-blue-cpp/
    $ cd feeling-blue-cpp
    $ mkdir build
    $ cd build
    $ cmake ..
    $ sudo make install

To use feeling-blue in your project, you gotta use cmake. Just call ``find_package``
and link the library. Here is what your CMakeLists.txt might look like:

.. code:: console

    cmake_minimum_required(VERSION 3.17)
    project(yourProject)

    set(CMAKE_CXX_STANDARD 17)

    find_package(feeling-blue REQUIRED)

    add_executable(yourProject main.cpp)
    target_link_libraries(yourProject PUBLIC feeling-blue)

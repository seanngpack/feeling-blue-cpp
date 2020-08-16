***************************
feeling-blue Documentation
***************************

.. image:: https://travis-ci.org/seanngpack/feeling-blue-cpp.svg?branch=master
    :target: https://travis-ci.org/seanngpack/feeling-blue-cpp
   :alt: Build Status
.. image:: https://img.shields.io/github/license/seanngpack/feeling-blue-cpp
    :target: https://github.com/seanngpack/feeling-blue-cpp
   :alt: license

feeling-blue_ was created because of a lack of resources for the non-expert on how
to connect to a bluetooth device using C++. The API was carefully designed to be easy-to-use,
yet feature-rich so you can take advantage of your bluetooth peripherals without learning
the nuisances of bluetoothLE. Okay, enough talking--let's get started!

.. _feeling-blue: http://www.github.com/seanngpack/feeling-blue-cpp

Installation
============

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


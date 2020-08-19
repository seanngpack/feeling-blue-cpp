# ![feeling-blue-logo](docs/img/feeling_blue_cpp.png)
C++ library for BluetoothLE usage on MacOS or (soon) Windows. 


feeling-blue was created because of a lack of resources for the non-expert on how
to connect to a bluetooth device using C++. The API was carefully designed to be easy-to-use,
yet feature-rich so you can take advantage of your bluetooth peripherals without learning
the nuisances of bluetoothLE.

[Full documentation here](https://seanngpack.github.io/feeling-blue-cpp/)

[![Build Status](https://travis-ci.com/seanngpack/feeling-blue-cpp.svg?branch=master)](https://travis-ci.com/seanngpack/feeling-blue-cpp)
![license](https://img.shields.io/github/license/seanngpack/feeling-blue-cpp)

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#Time-to-start-using-it)


---

## Features

* Blocking & non-blocking API. Calls to find services, characteristics, etc block the calling thread until the process is complete.
* Non-intrusive namespace. The API is encapsulated in ```bluetooth::``` so your global namespace doesn't get polluted.
* Read, write, and notify commands.
* User-defined callbacks on notifications received by device.
* Simple and straightfoward API.

## Installation

> Download and build
```
$ git clone https://github.com/seanngpack/feeling-blue-cpp
$ cd feeling-blue-cpp
$ mkdir build
$ cd build
$ cmake .. // use -DVERBOSE_MODE=OFF to turn off all console logging from feeling-blue
$ sudo make install
```

> Add library to your project, you just need to add ```find_package()``` and ```targeet_link_libraries()```.

* Example of cmakelists.txt in your project:

```
cmake_minimum_required(VERSION 3.17)
project(yourProject)

set(CMAKE_CXX_STANDARD 17)

find_package(feeling-blue REQUIRED)

add_executable(yourProject main.cpp)
target_link_libraries(yourProject PUBLIC feeling-blue)

```



## Time to start using it!
Below is a brief example that finds a device and reads, writes, and sets notifications to it. 

```
main.cpp

void print_data(std::vector<std::byte> data) {
    std::cout << "the size of the data is: " << data.size() << std::endl;
    for (auto const &b : data) {
        std::cout << (int)b << std::endl;
    }
}

#include "feeling-blue/feeling-blue.h"

int main() {
    std::unique_ptr<bluetooth::Central> central = std::make_unique<bluetooth::Central>();
    central->start_bluetooth();
    std::shared_ptr<bluetooth::Peripheral> peripheral = central->find_peripheral("SwagScanner");
    std::shared_ptr<bluetooth::Service> service = peripheral->find_service("5ffba521-2363-41da-92f5-46adc56b2d37");
    std::shared_ptr<bluetooth::Characteristic> rotate_char = service->find_characteristic("5ffba522-2363-41da-92f5-46adc56b2d37");
    std::shared_ptr<bluetooth::Characteristic> position_char = service->find_characteristic("5ffba523-2363-41da-92f5-46adc56b2d37");

    rotate_char->set_notify(print_data);
    std::vector<std::byte> data = position_char.read();
    rotate_char->write_without_response(data, 1);

    while (true) {
        ...
    }
  
    return 0;
}
```

[Go to the documentation for more comprehensive examples!](https://seanngpack.github.io/feeling-blue-cpp/)


## Upcoming features:
- more human readable way to send and receive data from characteristics
- methods to get status of peripherals, characteristics, etc
- better error logging
- Windows support

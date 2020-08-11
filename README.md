# ![feeling-blue-logo](doc/img/feeling_blue_cpp.png)
C++ library for BluetoothLE usage on MacOS or Windows. One of the only bluetooth libraries for MacOS.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#Time-to-start-using-it)


---

## Features

* Blocking API. Calls to find services, characteristics, etc block the calling thread until the process is complete.
Blocking methods are documented.
* Non-intrusive namespace. The API is encapsulated in ```bluetooth::``` so your global namespace does not get polluted.
* Simple and straightfoward API.

## Installation

> Download and build
```
$ git clone https://github.com/seanngpack/feeling-blue-cpp/tree/macos
$ cd feeling-blue-cpp
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
```

> Add library to your project

* Example of cmakelists.txt in your project:

```
cmake_minimum_required(VERSION 3.17)
project(yourProject)

set(CMAKE_CXX_STANDARD 17)

find_package(feeling-blue REQUIRED)

add_executable(yourProject main.cpp)
target_link_libraries(testLib PUBLIC feeling-blue)

```



## Time to start using it!
Below is an example of finding your device and connecting
to its service and characteristics.

```
main.cpp

#include "feeling-blue/feeling-blue.h"

int main() {
    std::unique_ptr<bluetooth::Central> central = std::make_unique<bluetooth::Central>();
    central->start_bluetooth();
    std::shared_ptr<bluetooth::Peripheral> peripheral = central->find_peripheral("SwagScanner");
    std::shared_ptr<bluetooth::Service> service = peripheral->find_service("5ffba521-2363-41da-92f5-46adc56b2d37");
    std::shared_ptr<bluetooth::Characteristic> rotate_char = service->find_characteristic("5ffba522-2363-41da-92f5-46adc56b2d37");
    std::shared_ptr<bluetooth::Characteristic> position_char = service->find_characteristic("5ffba523-2363-41da-92f5-46adc56b2d37");

    // some mechanism to keep your main thread alive
  
    return 0;
}
```

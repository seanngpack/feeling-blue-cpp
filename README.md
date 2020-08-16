# ![feeling-blue-logo](docs/img/feeling_blue_cpp.png)
C++ library for BluetoothLE usage on MacOS or Windows. 


feeling-blue was created because of a lack of resources for the non-expert on how
to connect to a bluetooth device using C++. The API was carefully designed to be easy-to-use,
yet feature-rich so you can take advantage of your bluetooth peripherals without learning
the nuisances of bluetoothLE.

[Full documentation here](https://seanngpack.github.io/feeling-blue-cpp/)

![TravisCI](https://img.shields.io/travis/seanngpack/feeling-blue-cpp)

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#Time-to-start-using-it)


---

## Features

* Blocking API. Calls to find services, characteristics, etc block the calling thread until the process is complete.
Blocking methods are documented.
* Non-intrusive namespace. The API is encapsulated in ```bluetooth::``` so your global namespace does not get polluted.
* User-defined callbacks on notifications received by device.
* Simple and straightfoward API.

## Installation

> Download and build
```
$ https://github.com/seanngpack/feeling-blue-cpp
$ cd feeling-blue-cpp
$ mkdir build
$ cd build
$ cmake ..
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
Below is a brief example of finding your device and connecting
to its service and characteristics. is_rotating_char supports notifications
so we enable them and pass a callback function to do something with the data
that it receives on notification.

```
main.cpp

void print_data(uint8_t *data) {
        int value;
        std::memcpy(&value, data, sizeof(int));
        std::cout << value << std::endl;
    }

#include "feeling-blue/feeling-blue.h"

int main() {
    std::unique_ptr<bluetooth::Central> central = std::make_unique<bluetooth::Central>();
    central->start_bluetooth();
    std::shared_ptr<bluetooth::Peripheral> peripheral = central->find_peripheral("SwagScanner");
    std::shared_ptr<bluetooth::Service> service = peripheral->find_service("5ffba521-2363-41da-92f5-46adc56b2d37");
    std::shared_ptr<bluetooth::Characteristic> rotate_char = service->find_characteristic("5ffba522-2363-41da-92f5-46adc56b2d37");
    std::shared_ptr<bluetooth::Characteristic> position_char = service->find_characteristic("5ffba523-2363-41da-92f5-46adc56b2d37");

    is_rotating_char->set_notify(print_data);

    // some mechanism to keep your main thread alive
  
    return 0;
}
```

Examples in ./examples go into more detail about how to use this library. 

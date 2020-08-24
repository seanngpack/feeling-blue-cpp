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
- [Upcoming Features](#Upcoming-features)
- [Help & bug reports](#Help-&-bug-reports)
- [Contributing](#Contributing)


---

## Features

* Blocking & non-blocking API. Calls to find services, characteristics, etc block the calling thread until the process is complete.
* Supports connecting to multiple bluetooth devices.
* User-defined event handlers for device notifications.
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

> Add library to your project, you just need to add ```find_package()``` and ```target_link_libraries()```.

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

void print_temp(std::vector<std::byte> data) {
    std::cout << "the size of the data is: " << data.size() << std::endl;
    for (auto const &b : data) {
        std::cout << (int)b << std::endl;
    }
    std::cout << "the current temperature is: " << bytes_to_int(data) << "degrees" << std::endl;
}

#include "feeling-blue/feeling-blue.h"

int main() {
    std::unique_ptr<bluetooth::Central> central = std::make_unique<bluetooth::Central>();
    central->start_bluetooth();
    std::shared_ptr<bluetooth::Peripheral> temperature_device = central->find_peripheral("NameOfYourDevice");
    std::shared_ptr<bluetooth::Service> service = temperature_device->find_service("someUUID");
    std::shared_ptr<bluetooth::Characteristic> current_temp_char = service->find_characteristic("someUUID");
    std::shared_ptr<bluetooth::Characteristic> units_char = service->find_characteristic("someUUID");

    current_temp_char->set_notify(print_temp); // will call print_temp() whenever device notifies
    
    std::string units = units_char.read<std::string>();
    std::cout << "the current temperature unit is: " << units << std::endl
    units_char->write_without_response<std::string>("kelvin"); // set new temperature unit to device
    units = units_char->read<std::string>();
    std::cout << "the new temperature unit is: " << units << std::endl

    while (true) {
        ...
    }
  
    return 0;
}
```

[Go to the documentation for more comprehensive examples!](https://seanngpack.github.io/feeling-blue-cpp/)


## Upcoming features:
- methods to get status of peripherals, characteristics, etc
- better error logging
- Windows support

## Help & bug reports

Just open a new [issue](https://github.com/seanngpack/feeling-blue-cpp/issues)!


## Contributing

1. Fork the repo
2. Clone your fork somewhere on your computer ```git clone https://github.com/<your-username>/pcl.git```
3. ```cd feeling-blue-cpp```
4. add upstream to original repo ```git remote add upstream https://github.com/seanngpack/feeling-blue-cpp.git```
5. implement your new changes on your fork
6. pull from upstream master to make sure you are up to date with the newest release of feeling-blue-cpp
7. open pull request

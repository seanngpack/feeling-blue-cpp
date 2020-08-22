#include <iostream>
#include <chrono>
#include <thread>
#include <iostream>
#include "feeling-blue/feeling-blue.h"

/**
 * This example demonstrates usage of a fictional smart temperature meter. This meter provides a service that contains two
 * characteristics
 *
 * battery_level_char: Configured with notification and read permissions on the device's end. You will use feeling-blue to receive notifications
 * from this characteristic. You will also utilize the read() method because you are impatient and want to get the battery
 * information right away.
 *
 * temp_units_char: Configured with read and write permissions on the device's end and defaulted to contain a value of "celcius".
 * You will write the string "kelvin" to this characteristic to change its value.
 *
 *
 */


float bytes_to_float(std::vector<std::byte> bytes) {
    float f;
    memcpy(&f, bytes.data(), sizeof(f));
    return f;
}


void print_data(std::vector<std::byte> data) {
    std::cout << "the size of the notified data is: " << data.size() << std::endl;
    std::cout << "here are the bytes of your data: ";
    for (auto const &b : data) {
        std::cout << (int) b << " ";
    }
    std::cout << std::endl;
    std::cout << "the current temperature is: " << bytes_to_float(data) << std::endl;
}


int main() {
    std::string peripheral_name = "SwagTemperature";
    std::string service_uuid = "19B10000-E8F2-537E-4F6C-D104768A1214";
    std::string battery_level_uuid = "19B10001-E8F2-537E-4F6C-D104768A1214";
    std::string temp_units_uuid = "19B10002-E8F2-537E-4F6C-D104768A1214";
    std::string current_temp_uuid = "19B10003-E8F2-537E-4F6C-D104768A1214";

    // Connection
    std::unique_ptr<bluetooth::Central> central = std::make_unique<bluetooth::Central>();
    central->start_bluetooth();
    std::shared_ptr<bluetooth::Peripheral> peripheral = central->find_peripheral(peripheral_name);
    std::shared_ptr<bluetooth::Service> service = peripheral->find_service(service_uuid);
    std::shared_ptr<bluetooth::Characteristic> battery_level_char = service->find_characteristic(
            battery_level_uuid);
    std::shared_ptr<bluetooth::Characteristic> temp_units_char = service->find_characteristic(temp_units_uuid);
    std::shared_ptr<bluetooth::Characteristic> current_temp_char = service->find_characteristic(current_temp_uuid);

    // get notifications whenever the current temperature characteristic updates
    // handle the data with print_data()
    current_temp_char->notify(print_data);

    // or read it without waiting for updates
    std::cout << "the current temperature is: " << current_temp_char->read_float() << std::endl;

    // read the battery level
    std::vector<std::byte> a = battery_level_char->read();
    std::cout << "printing bytes of battery" <<std::endl;
    for (auto &x: a) {
        std::cout << " " << (int)x;
    }
    std::cout << std::endl;
    std::cout << "the battery level is: " << battery_level_char->read_int() << std::endl;

    // current value is "fahrenheit"
    std::string unit = temp_units_char->read_string();
    std::cout << "the current unit is: " << unit << std::endl;
    // write to the characteristic to change units.
    temp_units_char->write_with_response("celsius");


    using namespace std::chrono_literals;
    // use while loop or some other mechanism to keep the main thread alive.
    while (true) {
        std::this_thread::sleep_for(1s);
        std::cout << "working.." << std::endl;
    }

    return 0;
}
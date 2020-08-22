#include <iostream>
#include <chrono>
#include <thread>
#include <iostream>
#include "feeling-blue/feeling-blue.h"

/**
 * This example demonstrates usage of a fictional smart temperature meter. This meter provides a service that contains two
 * characteristics.
 * This examples goes a step ahead of temperature_device.cpp by demonstrating usage of feeling-blue inside a class.
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


class SomeClass {
public:

    SomeClass() {
        std::string peripheral_name = "SwagTemperature";
        std::string service_uuid = "19B10000-E8F2-537E-4F6C-D104768A1214";
        std::string battery_level_uuid = "19B10001-E8F2-537E-4F6C-D104768A1214";
        std::string temp_units_uuid = "19B10002-E8F2-537E-4F6C-D104768A1214";
        std::string current_temp_uuid = "19B10003-E8F2-537E-4F6C-D104768A1214";

        central = std::make_unique<bluetooth::Central>();
        central->start_bluetooth();
        peripheral = central->find_peripheral(peripheral_name);
        service = peripheral->find_service(service_uuid);
        battery_level_char = service->find_characteristic(battery_level_uuid);
        current_temp_char = service->find_characteristic(current_temp_uuid);
        temp_units_char = service->find_characteristic(temp_units_uuid);

        // start notifications! Notice the binding syntax.
        using namespace std::placeholders;
        std::function<void(std::vector<std::byte>)> f = std::bind(&SomeClass::print_data, this, std::placeholders::_1);
        current_temp_char->notify(f);
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

    float bytes_to_float(std::vector<std::byte> bytes) {
        float f;
        memcpy(&f, bytes.data(), sizeof(f));
        return f;
    }

    int read_temp() {
        return current_temp_char->read_int();
    }

    void set_temp_units(const std::string &units) {
        temp_units_char->write_with_response(units);
    }

    std::string get_temp_units() {
        return temp_units_char->read_string();
    }



private:
    std::shared_ptr<bluetooth::Central> central;
    std::shared_ptr<bluetooth::Peripheral> peripheral;
    std::shared_ptr<bluetooth::Service> service;
    std::shared_ptr<bluetooth::Characteristic> battery_level_char;
    std::shared_ptr<bluetooth::Characteristic> current_temp_char;
    std::shared_ptr<bluetooth::Characteristic> temp_units_char;
};


int main() {

    SomeClass swag;

    // current value is "celcius"
    std::cout << "the current temperature unit is: " << swag.get_temp_units() << std::endl;
    // write to the characteristic to change units.
    swag.set_temp_units("celsius");
    // verify the new value is "kelvin"
    std::cout << "the new units is: " << swag.get_temp_units() << std::endl;


    using namespace std::chrono_literals;
    // use while loop or some other mechanism to keep the main thread alive.
    while (true) {
        std::this_thread::sleep_for(1s);
        std::cout << "working.." << std::endl;
    }

    return 0;
}
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


using namespace std::chrono_literals;

void print_data(std::vector<std::byte> data) {
    std::cout << "the size of the notified data is: " << data.size() << std::endl;
    std::cout << "here are the bytes of your data: ";
    for (auto const &b : data) {
        std::cout << (int) b << " ";
    }
    std::cout << std::endl;
}


int main() {
    std::string peripheral_name = "SmartTemperatureMeter";
    std::string service_uuid = "someUUID";
    std::string battery_level_char_uuid = "someUUID";
    std::string temp_units_uuid = "someUUID";

    // Connection
    std::unique_ptr<bluetooth::Central> central = std::make_unique<bluetooth::Central>();
    central->start_bluetooth();
    std::shared_ptr<bluetooth::Peripheral> peripheral = central->find_peripheral(peripheral_name);
    std::shared_ptr<bluetooth::Service> service = peripheral->find_service(service_uuid);
    std::shared_ptr<bluetooth::Characteristic> battery_level_char = service->find_characteristic(
            battery_level_char_uuid);
    std::shared_ptr<bluetooth::Characteristic> temp_units_char = service->find_characteristic(temp_units_uuid);

    // get notifications whenever the temperature characteristic updates
    // handle the data with print_data()
    battery_level_char->notify(print_data);

    // or read it without waiting for updates
    std::cout << "the battery level is: " << battery_level_char->read_int() << std::endl;

    // current value is "celcius"
    std::cout << "the current unit is: " << temp_units_char->read_string() << std::endl;
    // write to the characteristic to change units.
    temp_units_char->write_with_response("kelvin");
    // verify the new value is "kelvin"
    std::cout << "the new units is: " << temp_units_char->read_string() << std::endl;

    // use while loop or some other mechanism to keep the main thread alive.
    while (true) {
        std::this_thread::sleep_for(1s);
        std::cout << "working.." << std::endl;
    }

    return 0;
}
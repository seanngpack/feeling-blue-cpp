#include <iostream>
#include <chrono>
#include <thread>
#include <iostream>
#include "feeling-blue/feeling-blue.h"

using namespace std::chrono_literals;

// this is your callback function for .notify().
// must take in a uint8_t *data.
// Up to you how you want to break down the byte array.
// I know that SwagScanner is sending 8 bytes... but because the order is little-endian
// we can memcpy it into an array and forget about the other 4 bytes. This is not safe, but it
// demonstrates how you should know the length, size, and structure of the data you're sending/receiving.
// Then we can do something with the result, in this case we're going to print it.
void print_data(uint8_t *data) {
    int value;
    std::memcpy(&value, data, sizeof(int));
    std::cout << value << std::endl;
}

int main() {
    std::unique_ptr<bluetooth::Central> central = std::make_unique<bluetooth::Central>();
    central->start_bluetooth();
    std::shared_ptr<bluetooth::Peripheral> peripheral = central->find_peripheral("SwagScanner");
    std::shared_ptr<bluetooth::Service> service = peripheral->find_service("5ffba521-2363-41da-92f5-46adc56b2d37");
    std::shared_ptr<bluetooth::Characteristic> rotate_char = service->find_characteristic(
            "5ffba522-2363-41da-92f5-46adc56b2d37");
    std::shared_ptr<bluetooth::Characteristic> position_char = service->find_characteristic(
            "5ffba523-2363-41da-92f5-46adc56b2d37");
    std::shared_ptr<bluetooth::Characteristic> is_rotating_char = service->find_characteristic(
            "5ffba524-2363-41da-92f5-46adc56b2d37");

    // we pass the callback function we defined above into the set_notify method.
    is_rotating_char->set_notify(print_data);

    // here is a demo of writing to your device.
    // as documented, write_with_response attempts to write to your device, and blocks the calling
    // thread until the write is sucessful.
    uint8_t rotate_by = 9;
    auto *rotate_by_data = (uint8_t *) &rotate_by;
    rotate_char->write_with_response(rotate_by_data, 1);

    // instead of waiting for notifications you can also just read your characteristic.
    uint8_t *data = is_rotating_char->read();

    // use while loop or some other mechanism to keep the main thread alive.
    while (true) {
        std::this_thread::sleep_for(1s);
        std::cout << "working.." << std::endl;
    }

    return 0;
}
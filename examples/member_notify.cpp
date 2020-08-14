#include <iostream>
#include <chrono>
#include <thread>
#include <iostream>
#include "feeling-blue/feeling-blue.h"

using namespace std::chrono_literals;

// If you want to want to pass a member function to notify() then this class provides
// a slightly clunkly example of how to do it. The main idea is you just bind your class
// to std::function then pass it.
class A {
public:
    void print_data(uint8_t *data) {
        int value;
        std::memcpy(&value, data, sizeof(int));
        std::cout << value << std::endl;
    }

    void set_notify(std::shared_ptr<bluetooth::Characteristic> c) {
        using namespace std::placeholders;
        // The main takeaway from this example is that you must bind your class to the function if you
        // want to pass a member function to notify().
        // That's it.
        std::function<void(uint8_t *data)> f = std::bind(&A::print_data, this, std::placeholders::_1);
        c->notify(f);
    }
};


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

    std::unique_ptr<A> a = std::make_unique<A>();
    a->set_notify(is_rotating_char);

    // use while loop or some other mechanism to keep the main thread alive.
    while (true) {
        std::this_thread::sleep_for(1s);
        std::cout << "working.." << std::endl;
    }

    return 0;
}
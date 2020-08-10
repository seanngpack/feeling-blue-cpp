#include "wrapper.h"
#include "peripheral.h"
#include "central_event_handler.h"

#include <utility>

handler::CentralEventHandler::CentralEventHandler(std::shared_ptr<wrapper::Wrapper> bluetooth) :
        bluetooth_object(std::move(bluetooth)),
        is_powered_on(false),
        is_peripheral_found(false) {
    bluetooth_object->set_handler(this);
}

void handler::CentralEventHandler::start_bluetooth() {
    std::unique_lock<std::mutex> ul(power_mutex);
    bluetooth_object->start_bluetooth();
    power_cv.wait(ul, [this]() { return is_powered_on; }); // wait until is_powered_on is true

    std::cout << "Finally powered on, unblocking thread" << std::endl;
//    is_powered_on = false; // this will be needed when I refactor to one mutex
}

bluetooth::Peripheral handler::CentralEventHandler::find_peripheral(const std::vector<std::string> &uuids) {
    bluetooth_object->find_peripheral(uuids);
    bluetooth::Peripheral *p = bluetooth_object->get_peripheral();
    p->set_bluetooth(bluetooth_object);
    return *p;
}

bluetooth::Peripheral handler::CentralEventHandler::find_peripheral(const std::string &name) {
    bluetooth_object->find_peripheral(name);
    bluetooth::Peripheral *p = bluetooth_object->get_peripheral();
    p->set_bluetooth(bluetooth_object);
    return *p;
}

void handler::CentralEventHandler::rotate_by(int degs) {
    using namespace std::literals::chrono_literals;
    std::unique_lock<std::mutex> ul(peripheral_mutex);
//    rotate(bluetooth_object, degs);

    peripheral_cv.wait(ul, [this]() { return !is_peripheral_found; });
    std::this_thread::sleep_for(.5s);
}


void handler::CentralEventHandler::set_is_powered_on(bool connected) {
    is_powered_on = connected;
}

void handler::CentralEventHandler::set_is_peripheral_found(bool found) {
    is_peripheral_found = found;
}

handler::CentralEventHandler::~CentralEventHandler() {
    bluetooth_object = nullptr;
}






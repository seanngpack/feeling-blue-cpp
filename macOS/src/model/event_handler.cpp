#include "wrapper.h"
#include "peripheral.h"
#include "event_handler.h"

#include <utility>

bluetooth::handler::EventHandler::EventHandler(std::shared_ptr<bluetooth::wrapper::Wrapper> bluetooth) :
        bluetooth_object(std::move(bluetooth)),
        proceed(false) {
    bluetooth_object->set_handler(this);
}

void bluetooth::handler::EventHandler::start_bluetooth() {
    std::unique_lock<std::mutex> ul(mut);
    bluetooth_object->start_bluetooth();
    cv.wait(ul, [this]() { return proceed; }); // wait until proceed is true
    std::cout << "Finally powered on, unblocking thread" << std::endl;
    proceed = false;
}

bluetooth::Peripheral *bluetooth::handler::EventHandler::find_peripheral(const std::vector<std::string> &uuids) {
    std::unique_lock<std::mutex> ul(mut);
    bluetooth_object->find_peripheral(uuids);
    cv.wait(ul, [this]() { return proceed; }); // wait until proceed is true
    proceed = false;
    auto *p = new bluetooth::Peripheral(bluetooth_object->get_peripheral_name(),bluetooth_object, this);
    return p;
}

bluetooth::Peripheral *bluetooth::handler::EventHandler::find_peripheral(const std::string &name) {
    std::unique_lock<std::mutex> ul(mut);
    bluetooth_object->find_peripheral(name);
    cv.wait(ul, [this]() { return proceed; }); // wait until proceed is true
    proceed = false;
    auto *p = new bluetooth::Peripheral(bluetooth_object->get_peripheral_name(),bluetooth_object, this);
    return p;
}

void bluetooth::handler::EventHandler::rotate_by(int degs) {
    using namespace std::literals::chrono_literals;
    std::unique_lock<std::mutex> ul(mut);
//    rotate(bluetooth_object, degs);

    cv.wait(ul, [this]() { return !proceed; });
    std::this_thread::sleep_for(.5s);
}


void bluetooth::handler::EventHandler::set_proceed(bool connected) {
    proceed = connected;
}

bluetooth::handler::EventHandler::~EventHandler() {
    bluetooth_object = nullptr;
}






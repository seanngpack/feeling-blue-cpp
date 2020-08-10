#include "wrapper.h"
#include "peripheral.h"
#include "central_event_handler.h"

#include <utility>

bluetooth::handler::CentralEventHandler::CentralEventHandler(std::shared_ptr<bluetooth::wrapper::Wrapper> bluetooth) :
        bluetooth_object(std::move(bluetooth)),
        proceed(false) {
    bluetooth_object->set_handler(this);
}

void bluetooth::handler::CentralEventHandler::start_bluetooth() {
    std::unique_lock<std::mutex> ul(central_mutex);
    bluetooth_object->start_bluetooth();
    cv.wait(ul, [this]() { return proceed; }); // wait until proceed is true
    std::cout << "Finally powered on, unblocking thread" << std::endl;
    proceed = false;
}

bluetooth::Peripheral bluetooth::handler::CentralEventHandler::find_peripheral(const std::vector<std::string> &uuids) {
    std::unique_lock<std::mutex> ul(central_mutex);
    bluetooth_object->find_peripheral(uuids);
    cv.wait(ul, [this]() { return proceed; }); // wait until proceed is true
    proceed = false;
    bluetooth::Peripheral *p = bluetooth_object->get_peripheral();
    p->set_bluetooth(bluetooth_object);
    return *p;
}

bluetooth::Peripheral bluetooth::handler::CentralEventHandler::find_peripheral(const std::string &name) {
    std::unique_lock<std::mutex> ul(central_mutex);
    bluetooth_object->find_peripheral(name);
    cv.wait(ul, [this]() { return proceed; }); // wait until proceed is true
    proceed = false;
    bluetooth::Peripheral *p = bluetooth_object->get_peripheral();
    p->set_bluetooth(bluetooth_object);
    return *p;
}

void bluetooth::handler::CentralEventHandler::rotate_by(int degs) {
    using namespace std::literals::chrono_literals;
    std::unique_lock<std::mutex> ul(central_mutex);
//    rotate(bluetooth_object, degs);

    cv.wait(ul, [this]() { return !proceed; });
    std::this_thread::sleep_for(.5s);
}


void bluetooth::handler::CentralEventHandler::set_proceed(bool connected) {
    proceed = connected;
}

bluetooth::handler::CentralEventHandler::~CentralEventHandler() {
    bluetooth_object = nullptr;
}






#include "wrapper.h"
#include "peripheral.h"
#include "service.h"
#include "characteristic.h"
#include "event_handler.h"
#include <iostream>
#include <utility>

bluetooth::handler::EventHandler::EventHandler() :
        bluetooth_object(std::make_unique<wrapper::Wrapper>()),
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

std::shared_ptr<bluetooth::Peripheral>
bluetooth::handler::EventHandler::find_peripheral(const std::vector<std::string> &uuids) {
    std::unique_lock<std::mutex> ul(mut);
    bluetooth_object->find_peripheral(uuids);
    cv.wait(ul, [this]() { return proceed; }); // wait until proceed is true
    proceed = false;
    return std::make_shared<bluetooth::Peripheral>(bluetooth_object->get_peripheral_name(), this);
}

std::shared_ptr<bluetooth::Peripheral> bluetooth::handler::EventHandler::find_peripheral(const std::string &name) {
    std::unique_lock<std::mutex> ul(mut);
    bluetooth_object->find_peripheral(name);
    cv.wait(ul, [this]() { return proceed; }); // wait until proceed is true
    proceed = false;
    return std::make_shared<bluetooth::Peripheral>(bluetooth_object->get_peripheral_name(), this);;
}

std::shared_ptr<bluetooth::Service> bluetooth::handler::EventHandler::find_service(const std::string &uuid) {
    std::unique_lock<std::mutex> ul(mut);
    bluetooth_object->find_service(uuid);
    cv.wait(ul, [this]() { return proceed; }); // wait until proceed is true
    proceed = false;

    std::shared_ptr<bluetooth::Service> s;

    if (service_found) {
        s = std::make_shared<bluetooth::Service>(uuid, this);
    } else {
        s = nullptr;
    }

    // reset this flag for future use.
    service_found = false;

    return s;
}

std::shared_ptr<bluetooth::Characteristic> bluetooth::handler::EventHandler::find_characteristic(const std::string &char_uuid,
                                                                                 const std::string &service_uuid) {
    std::unique_lock<std::mutex> ul(mut);
    bluetooth_object->find_characteristic(char_uuid, service_uuid);
    cv.wait(ul, [this]() { return proceed; }); // wait until proceed is true
    proceed = false;

    std::shared_ptr<Characteristic> c;

    if (char_found) {
        c = std::make_shared<Characteristic>(char_uuid, service_uuid, this);
    } else {
        c = nullptr;
    }

    // reset this flag for future use.
    char_found = false;

    return c;
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



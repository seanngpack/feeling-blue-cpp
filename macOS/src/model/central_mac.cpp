#include "central_mac.h"
#include <iostream>

bluetooth::CentralMac::CentralMac() :
        bluetooth_object(std::make_shared<wrapper::Wrapper>()),
        event_handler(new handler::CentralEventHandler(bluetooth_object)) {}

bluetooth::PeripheralMac bluetooth::CentralMac::find_peripheral(std::vector<std::string> uuids) {
    return event_handler->find_peripheral(uuids);
}

bluetooth::PeripheralMac bluetooth::CentralMac::find_peripheral(std::string name) {
    return event_handler->find_peripheral(name);
}



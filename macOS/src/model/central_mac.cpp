#include "central_mac.h"
#include <iostream>

bluetooth::PeripheralMac bluetooth::CentralMac::find_peripheral(std::vector<std::string> uuids) {
    return bluetooth::PeripheralMac();
}

bluetooth::PeripheralMac bluetooth::CentralMac::find_peripheral(std::string name) {
    return bluetooth::PeripheralMac();
}

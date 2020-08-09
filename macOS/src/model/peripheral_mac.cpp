#include "peripheral_mac.h"


bluetooth::PeripheralMac::PeripheralMac() {

}

void bluetooth::PeripheralMac::set_name(std::string name) {
    this->name = name;
}

void bluetooth::PeripheralMac::set_bluetooth(std::shared_ptr<wrapper::Wrapper> bt) {
    this->bluetooth = bt;

}

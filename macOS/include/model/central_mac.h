#ifndef FEELING_BLUE_CENTRAL_MAC_H
#define FEELING_BLUE_CENTRAL_MAC_H

#include "peripheral_impl.h"
#include "central_event_handler.h"
#include <vector>
#include <string>

namespace bluetooth {
    class CentralMac {
    public:
        CentralMac();


        /**
         * Find and connect to the peripheral_mac given the service UUIDs advertised by the peripheral_mac.
         * @param uuids
         */
        PeripheralImpl find_peripheral(std::vector<std::string> uuids);

        /**
         * Find and connect to a peripheral_mac given the name of it.
         * @param name the name advertised by the peripheral_mac.
         */
        PeripheralImpl find_peripheral(std::string name);


    private:
        std::shared_ptr<wrapper::Wrapper> bluetooth_object;
        handler::CentralEventHandler *event_handler;
    };
}

#endif //FEELING_BLUE_CENTRAL_DETAILS_H

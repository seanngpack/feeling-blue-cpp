#ifndef FEELING_BLUE_PERIPHERAL_MAC_H
#define FEELING_BLUE_PERIPHERAL_MAC_H

#include "wrapper.h"
#include <string>

namespace bluetooth {
    class PeripheralMac {
    public:
        PeripheralMac();

        void set_name(std::string name);

        void set_bluetooth(std::shared_ptr<wrapper::Wrapper> bt);

    private:
        std::shared_ptr<wrapper::Wrapper> bluetooth;
        std::string name;

    };
}

#endif //FEELING_BLUE_PERIPHERAL_MAC_H

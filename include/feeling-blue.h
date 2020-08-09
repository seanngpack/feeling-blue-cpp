#ifndef FEELING_BLUE_FEELING_BLUE_H
#define FEELING_BLUE_FEELING_BLUE_H

#if __APPLE__

#include "central_mac.h"
#include "peripheral_impl.h"

namespace bluetooth {
    typedef bluetooth::CentralMac Central;
    typedef bluetooth::PeripheralMac Peripheral;
}

#endif


#endif //FEELING_BLUE_FEELING_BLUE_H

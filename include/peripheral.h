#ifndef FEELING_BLUE_PERIPHERAL_H
#define FEELING_BLUE_PERIPHERAL_H

#include <iostream>

namespace bluetooth {
    class Wrapper;

    class Peripheral {
    public:

        Peripheral();

        ~Peripheral();

        void set_name(const std::string& name);

        void set_bluetooth(std::shared_ptr<Wrapper> bt);

    private:
        struct PeripheralImpl;
        PeripheralImpl *pImpl;
    };
}

#endif //FEELING_BLUE_PERIPHERAL_H

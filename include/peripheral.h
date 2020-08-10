#ifndef FEELING_BLUE_PERIPHERAL_H
#define FEELING_BLUE_PERIPHERAL_H

#include <string>


namespace bluetooth {

    // forward declare here is exposing wrapper
    // might want to choose a different namespace.
    // or make wrapper a subclass
    // or add a helper method in the implementation file
    // or ask stackoverflow
    // or this might be fine tbh, they can't do anything with wrapper.
    namespace wrapper {
        class Wrapper;
    }

    class Peripheral {
    public:

        Peripheral();

        ~Peripheral();

        void set_name(const std::string &name);

        void set_bluetooth(std::shared_ptr<wrapper::Wrapper> bt);

    private:
        struct PeripheralImpl;
        PeripheralImpl *pImpl;
    };
}

#endif //FEELING_BLUE_PERIPHERAL_H

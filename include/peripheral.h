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

    namespace handler {
        class EventHandler;
    }

    class Service;

    /**
     * represents a peripheral device.
     * Owned by central.
     */
    class Peripheral {
    public:

        Peripheral(const std::string &name, handler::EventHandler *event_handler);

        ~Peripheral();

        /**
         * Find service given the uuid.
         * @param uuid service's uuid.
         * @return the service, or nullptr if the service cannot be found.
         * TODO: I'm invoking the copy constructor to return a new object...is this appropriate
         * to return to the consumer?
         */
        Service find_service(const std::string &uuid);


    private:
        struct PeripheralImpl;
        PeripheralImpl *pImpl;
    };
}

#endif //FEELING_BLUE_PERIPHERAL_H

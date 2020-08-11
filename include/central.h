#ifndef FEELING_BLUE_CENTRAL_H
#define FEELING_BLUE_CENTRAL_H

#include <vector>
#include <string>
#include <memory>

namespace bluetooth {
    class Peripheral;

    /**
     * Represents a central manager.
     */
    class Central {
    public:
        Central();

        ~Central();

        void start_bluetooth();

        /**
         * Find and connect to the peripheral_mac given the service UUIDs advertised by the peripheral_mac.
         * @param uuids uuids advertised by the peripheral.
         */
        std::shared_ptr<Peripheral> find_peripheral(const std::vector<std::string> &uuids);

        /**
         * Find and connect to a peripheral_mac given the name of it.
         * @param name the name advertised by the peripheral_mac.
         */
        std::shared_ptr<Peripheral> find_peripheral(const std::string &name);


    private:
        struct CentralImpl;
        CentralImpl *cImpl;
    };
}

#endif //FEELING_BLUE_CENTRAL_DETAILS_H


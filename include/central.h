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

        /**
         * Start bluetooth on your computer.
         */
        void start_bluetooth();

        /**
         * Find and connect to the peripheral_mac given the service UUIDs advertised by the peripheral_mac.
         * If the peripheral is not found, return nullptr.
         *
         * @param uuids uuids advertised by the peripheral.
         */
        std::shared_ptr<Peripheral> find_peripheral(const std::vector<std::string> &uuids);

        /**
         * Find and connect to a peripheral_mac given the name of it.
         * If the peripheral is not found, return nullptr.
         *
         * @param name the name advertised by the peripheral_mac.
         */
        std::shared_ptr<Peripheral> find_peripheral(const std::string &name);

        /**
         * Return the discovered peripheral. If the peripheral hasn't been discovered and connected yet
         * then return nullptr.
         * @param uuid uuid of the peripheral.
         * @return the peripheral or nullptr.
         */
        std::shared_ptr<Peripheral> peripheral(const std::string &uuid);


    private:
        struct CentralImpl;
        CentralImpl *cImpl;
    };
}

#endif //FEELING_BLUE_CENTRAL_DETAILS_H


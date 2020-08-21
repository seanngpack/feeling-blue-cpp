#ifndef FEELING_BLUE_PERIPHERAL_H
#define FEELING_BLUE_PERIPHERAL_H

#include <string>
#include <memory>


namespace bluetooth {

    namespace wrapper {
        class Wrapper;
    }

    class Service;

    /**
     * represents a peripheral device.
     */
    class Peripheral {
    public:

        Peripheral(const std::string &name, std::shared_ptr<wrapper::Wrapper> bt);

        ~Peripheral();

        /**
         * Find advertised service given the service_uuid. Return the service if found, otherwise returns nullptr.
         *
         * @param service_uuid service uuid.
         * @return the service or nullptr.
         */
        std::shared_ptr<Service> find_service(const std::string &service_uuid);

        /**
         * Returns the discovered service. If the service has not been found and connected via find_service() yet,
         * then this will return nullptr.
         *
         * @param service_uuid service uuid.
         * @return service matching the service_uuid or nullptr.
         */
        std::shared_ptr<Service> service(const std::string &service_uuid);

        /**
         * Disconnect the peripheral.
         *
         * @note not blocking.
         */
        void disconnect();

        /**
         * Get the name of your peripheral.
         * Will return empty if you discovered your peripheral via UUID.
         *
         * @return the name of your peripheral.
         */
        std::string name();


    private:
        struct PeripheralImpl;
        PeripheralImpl *pImpl;
    };
}

#endif //FEELING_BLUE_PERIPHERAL_H

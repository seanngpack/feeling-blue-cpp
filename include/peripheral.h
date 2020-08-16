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
         * After discovering and connecting to services, will store them into a list. This method
         * matches the given service_uuid and returns the corresponding service.
         * If the service is not found in the peripheral, then return nullptr.
         *
         * @param service_uuid service uuid.
         * @return service matching the service_uuid or nullptr.
         */
        std::shared_ptr<Service> get_service(const std::string &service_uuid);

        /**
         * Set the name for your peripheral device. Use this if you discovered your peripheral via
         * find_peripheral(std::vector<std::string) because the name functionality is bugged right now.
         *
         * @param n the name of your peripheral.
         */
        void set_name(const std::string &n);

        /**
         * Get the name of your peripheral.
         *
         * @return the name of your peripheral.
         */
        std::string get_name();


    private:
        struct PeripheralImpl;
        PeripheralImpl *pImpl;
    };
}

#endif //FEELING_BLUE_PERIPHERAL_H

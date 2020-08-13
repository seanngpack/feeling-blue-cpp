#ifndef FEELING_BLUE_PERIPHERAL_H
#define FEELING_BLUE_PERIPHERAL_H

#include <string>
#include <memory>


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
         * @param service_uuid service's service_uuid.
         * @return the service or nullptr.
         */
        std::shared_ptr<Service> find_service(const std::string &service_uuid);

        /**
         * After discovering and connecting to services, will store them into a list. This method
         * matches the given service_uuid and returns the corresponding service.
         * If the service is not found in the peripheral, then return nullptr.
         * @return service matching the service_uuid or nullptr.
         */
        std::shared_ptr<Service> get_service(const std::string &service_uuid);


    private:
        struct PeripheralImpl;
        PeripheralImpl *pImpl;
    };
}

#endif //FEELING_BLUE_PERIPHERAL_H

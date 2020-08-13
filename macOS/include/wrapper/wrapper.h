#ifndef FEELING_BLUE_WRAPPER_H
#define FEELING_BLUE_WRAPPER_H


#include <vector>
#include <string>
#include <memory>

namespace bluetooth {

    class Peripheral;

    class Service;

    namespace handler {
        class EventHandler;
    }

    namespace wrapper {
        struct WrapperImpl;

        class Wrapper {
        public:
            Wrapper();

            virtual ~Wrapper();

            /**
             * Initialize the central manager in a separate thread. Powers on bluetooth_object
             * central device. After this, you should scan for a peripheral device.
             */
            void start_bluetooth();

            /**
             * Find the peripheral using the advertised uuids.
             * @return true if the peripheral was found, false otherwise.
             */
            bool find_peripheral(std::vector<std::string> uuids);

            /**
             * Find the peripheral using the name of the device.
             * @return true if the peripheral was found, false otherwise.
             */
            bool find_peripheral(std::string name);

            /**
             * Find service given the service_uuid.
             * @param service_uuid service_uuid of the service
             * @return a pointer
             */
            bool find_service(std::string service_uuid);

            /**
             * Find the characteristic given the uuid of the characteristic and the
             * service that it belongs to.
             * @param char_uuid uuid of the characteristic.
             * @param service_uuid uuid of the service.
             * @return true if the characteristic is found, false otherwise.
             */
            bool find_characteristic(std::string char_uuid, std::string service_uuid);

            /**
             * Return the name of the connected peripheral.
             * @return name of peripheral.
             */
            std::string get_peripheral_name();

            uint8_t *read(const std::string &service_uuid, const std::string &char_uuid);

        private:
            WrapperImpl *impl;
        };
    }
}


#endif //FEELING_BLUE_WRAPPER_H

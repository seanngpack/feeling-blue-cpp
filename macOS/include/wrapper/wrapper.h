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
             * See Central method.
             */
            void find_peripheral(std::vector<std::string> uuids);

            /**
             * See Central method.
             */
            void find_peripheral(std::string name);

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
             */
            void find_characteristic(std::string char_uuid, std::string service_uuid);

            /**
             * Return the name of the connected peripheral.
             * @return name of peripheral.
             */
            std::string get_peripheral_name();

            uint8_t *read(const std::string &service_uuid, const std::string &char_uuid);

            /**
             * Calls on objective c function to set the callback to the arduino handler object.
             * @param event_handler the arduino handler object.
             * @param obj the wrapped object.
             */
            void set_handler(std::shared_ptr<handler::EventHandler> _event_handler);


        private:
            WrapperImpl *impl;
        };
    }
}


#endif //FEELING_BLUE_WRAPPER_H

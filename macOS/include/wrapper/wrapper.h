#ifndef FEELING_BLUE_WRAPPER_H
#define FEELING_BLUE_WRAPPER_H


#include <vector>
#include <string>

namespace bluetooth {

    class Peripheral;
    class Service;

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
             * Find service given the uuid.
             * @param uuid uuid of the service
             * @return a pointer
             */
            void *find_service(std::string uuid);

            /**
             * Return the name of the connected peripheral.
             * @return name of peripheral.
             */
            std::string get_peripheral_name();

            /**
             * Calls on objective c function to set the callback to the arduino handler object.
             * @param arduino the arduino handler object.
             * @param obj the wrapped object.
             */
            void set_handler(void *central_event_handler);


        private:
            WrapperImpl *impl;
        };
    }
}


#endif //FEELING_BLUE_WRAPPER_H

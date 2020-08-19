#ifndef FEELING_BLUE_WRAPPER_H
#define FEELING_BLUE_WRAPPER_H


#include <vector>
#include <string>
#include <memory>
#include <functional>

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
             * Find the peripheral using the advertised uuids.
             * @return true if the peripheral was found, false otherwise.
             */
            bool find_peripheral(const std::vector<std::string> &uuids);

            /**
             * Find the peripheral using the name of the device.
             * @return true if the peripheral was found, false otherwise.
             */
            bool find_peripheral(const std::string &name);

            /**
             * Find service given the service_uuid.
             * @param service_uuid service_uuid of the service
             * @return a pointer
             */
            bool find_service(const std::string &service_uuid);

            /**
             * Find the characteristic given the uuid of the characteristic and the
             * service that it belongs to.
             * @param char_uuid uuid of the characteristic.
             * @param service_uuid uuid of the service.
             * @return true if the characteristic is found, false otherwise.
             */
            bool find_characteristic(const std::string &char_uuid, const std::string &service_uuid);

            /**
             * Return the name of the connected peripheral.
             * @return name of peripheral.
             */
            std::string get_peripheral_name();

            /**
             * Read value from characteristics.
             * @param service_uuid service the characteristic belongs to.
             * @param char_uuid characteristic you want to read from.
             * @return byte vector.
             */
            std::vector<std::byte> read(const std::string &service_uuid, const std::string &char_uuid);

            void write_without_response(const std::vector<std::byte> &data,
                                        const std::string &service_uuid,
                                        const std::string &char_uuid);

            void write_with_response(const std::vector<std::byte> &data,
                                     const std::string &service_uuid,
                                     const std::string &char_uuid);

            void notify(const std::string &service_uuid,
                        const std::string &char_uuid,
                        const std::function<void(std::vector<std::byte>)> &callback);

            /**
             * Disconnect the peripheral.
             */
            void disconnect();


        private:
            WrapperImpl *impl;
        };
    }
}


#endif //FEELING_BLUE_WRAPPER_H

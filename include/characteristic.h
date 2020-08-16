#ifndef FEELING_BLUE_CHARACTERISTIC_H
#define FEELING_BLUE_CHARACTERISTIC_H

#include <string>
#include <memory>
#include <functional>

namespace bluetooth {

    namespace wrapper {
        class Wrapper;
    }

    class Service;

    /**
     * Represents a characteristic.
     */
    class Characteristic {
    public:

        Characteristic(const std::string &char_uuid,
                       const std::string &service_uuid,
                       std::shared_ptr<wrapper::Wrapper> bt);

        ~Characteristic();

        std::string uuid();

        /**
         * Read the characteristic.
         * @return a byte vector.
         */
        std::vector<std::byte> read();

        /**
         * Write to characteristic. Performs this action asynchronously and will not print error if write fails.
         * You can transmit more data with this method than write_with_response().
         * @param data byte vector of your data.
         */
        void write_without_response(std::vector<std::byte> data);

        /**
         * Write to characteristic with response. Blocks calling thread until write_without_response is confirmed to
         * be either success or failure. If the write_without_response fails, the console will print an error and
         * the program will continue running.
         * @param byte vector of your data.
         */
        void write_with_response(std::vector<std::byte> data);

        /**
         * Enable notifications from the characteristic and set callback function to do something
         * with the data when the device notifies.
         * @param callback function to do something with notificaiton data.
         */
        void notify(const std::function<void(std::vector<std::byte>)> &callback);


    private:
        struct CharacteristicImpl;
        CharacteristicImpl *cImpl;
    };
}

#endif //FEELING_BLUE_CHARACTERISTIC_H

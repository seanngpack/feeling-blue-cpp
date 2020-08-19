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

        Characteristic() = default;

        Characteristic(const std::string &char_uuid,
                       const std::string &service_uuid,
                       std::shared_ptr<wrapper::Wrapper> bt);

        ~Characteristic();

        std::string uuid();

        /**
         * Read the characteristic.
         *
         * @return a byte vector.
         */
        std::vector<std::byte> read();

        /**
         * Write to characteristic using a vector of bytes. Will not print error if write fails.
         * You can transmit more data with this method than write_with_response().
         *
         * @note asynchronous.
         * @param data byte vector of your data.
         */
        void write_without_response(const std::vector<std::byte> &data);

        /**
         * Write integer to characteristic. Will not print error if write fails.
         * You can transmit more data with this method than write_with_response().
         *
         * @note asynchronous.
         * @param data int you want to send.
         */
        void write_without_response(int data);


        /**
         * Write uint8_t to characteristic. Will not print error if write fails.
         * You can transmit more data with this method than write_with_response().
         *
         * @note asynchronous.
         * @param data uint8_t you want to send.
         */
        void write_without_response(uint8_t data);

        /**
         * Write string to characteristic. Will not print error if write fails.
         * You can transmit more data with this method than write_with_response().
         *
         * @note asynchronous.
         * @param data string you want to write.
         */
        void write_without_response(const std::string &data);

        /**
         * Write to characteristic with response. If the write_without_response fails and verbose mode is on,
         * the console will print an error and the program will continue running.
         *
         * @param byte vector of your data.
         */
        void write_with_response(const std::vector<std::byte> &data);

        /**
         * Write to characteristic with response. If the write_without_response fails and verbose mode is on,
         * the console will print an error and the program will continue running.
         *
         * @param data int you want to send.
         */
        void write_with_response(int data);


        /**
         * Write to characteristic with response. If the write_without_response fails and verbose mode is on,
         * the console will print an error and the program will continue running.
         *
         * @param data uint8_t you want to send
         */
        void write_with_response(uint8_t data);

        /**
         * Write to characteristic with response. If the write_without_response fails and verbose mode is on,
         * the console will print an error and the program will continue running.
         *
         * @param data string you want to send
         */
        void write_with_response(const std::string &data);

        /**
         * Enable notifications from the characteristic and set callback function to do something
         * with the data when the device notifies.
         *
         * @note this method returns when the callback is sucessfully set. Your callback is asynchronously called whenever
         * a notification is triggered.
         * @param callback function to do something with notificaiton data.
         */
        void notify(const std::function<void(std::vector<std::byte>)> &callback);


    private:
        struct CharacteristicImpl;
        CharacteristicImpl *cImpl;
    };
}

#endif //FEELING_BLUE_CHARACTERISTIC_H

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

        Characteristic();

        Characteristic(const std::string &char_uuid,
                       const std::string &service_uuid,
                       std::shared_ptr<wrapper::Wrapper> bt);

        ~Characteristic();

        std::string uuid();

        /**
         * Read the characteristic.
         *
         * @note blocking.
         * @return a byte vector.
         */
        std::vector<std::byte> read();

        /**
         * Read the characteristic. Convert the byte response to integer assuming little endian order.
         * If your device sends more than four bytes, then it will use display a warning and use
         * the leftmost four bytes (little endian order) to convert to an int.
         *
         * @note blocking.
         * @return byte response as an integer.
         */
        int read_int();

        /**
         * Read the characteristic. Convert the byte response to uint8_t.
         * If your device sends more than one byte of data then it will display a warning
         * use the rightmost byte (little endian order) for the return.
         *
         * @note blocking.
         * @return byte response as uint8_t.
         */
        uint8_t read_uint8();

        /**
         * Read the characteristic. Convert the byte response to string assuming little endian order.
         *
         * @note blocking.
         * @return byte response as string.
         */
        std::string read_string();

        /**
         * Write to characteristic using a vector of bytes. Will not print error if write fails.
         * You can transmit more data with this method than write_with_response().
         *
         * @note asynchronous.
         * @param data byte vector of your data.
         */
        void write_without_response(const std::vector<std::byte> &data);

        /**
         * Write integer to characteristic in little endian order. Will not print error if write fails.
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
         * Write string to characteristic in little endian order. Will not print error if write fails.
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
         * Write to characteristic with response in little endian order. If the write_without_response fails and verbose mode is on,
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
         * Write to characteristic with response in little endian order. If the write_without_response fails and verbose mode is on,
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

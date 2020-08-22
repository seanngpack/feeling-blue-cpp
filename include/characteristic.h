#ifndef FEELING_BLUE_CHARACTERISTIC_H
#define FEELING_BLUE_CHARACTERISTIC_H

#include <string>
#include <memory>
#include <functional>

namespace bluetooth {

    namespace detail {
        namespace wrapper {
            class Wrapper;
        }
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
                       std::shared_ptr<detail::wrapper::Wrapper> bt);

        ~Characteristic();

        std::string uuid();

        /**
         * Read the characteristic.
         *
         * @tparam T the type your want the data to be read as. Currently supports:
         * std::byte
         * std::string
         * int
         * uint8_t
         * float
         *
         * @note blocking.
         * @return a byte vector.
         */
        template<typename T>
        T read();


        /**
         * Write to characteristic using a vector of bytes. Will not print error if write fails.
         * You can transmit more data with this method than write_with_response().
         *
         * @note asynchronous.
         * @param data byte vector of your data.
         */
        template<typename T>
        void write_without_response(T data);

        template<typename T>
        void write_without_response(const std::vector<T> &data);


        /**
         * Write to characteristic with response. If the write_without_response fails and verbose mode is on,
         * the console will print an error and the program will continue running.
         *
         * @param byte vector of your data.
         */
        void write_with_response(const std::vector<std::byte> &data);

        /**
         * Write float to characteristic with response.
         * If the write_without_response fails and verbose mode is on,
         * the console will print an error and the program will continue running.
         *
         * @param data int you want to send.
         */
        void write_with_response(float data);

        /**
         * Write integer to characteristic with response.
         * If the write_without_response fails and verbose mode is on,
         * the console will print an error and the program will continue running.
         *
         * @param data int you want to send.
         */
        void write_with_response(int data);


        /**
         * Write uint8_t to characteristic with response.
         * If the write_without_response fails and verbose mode is on,
         * the console will print an error and the program will continue running.
         *
         * @param data uint8_t you want to send
         */
        void write_with_response(uint8_t data);

        /**
         * Write string to characteristic with response.
         * If the write_without_response fails and verbose mode is on,
         * the console will print an error and the program will continue running.
         *
         * @param data string you want to send
         */
        void write_with_response(const std::string &data);

        /**
         * Enable notifications from the characteristic and set callback function to do something
         * with the data when the device notifies.
         *
         * @note this method returns when the callback is successfully set. Your callback is asynchronously called whenever
         * a notification is triggered.
         * @param callback function to do something with notification data.
         */
        void notify(const std::function<void(std::vector<std::byte>)> &callback);


    private:
        struct CharacteristicImpl;
        CharacteristicImpl *cImpl;
    };

}

#endif //FEELING_BLUE_CHARACTERISTIC_H

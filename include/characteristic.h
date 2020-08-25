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
         * Compatible template types are listed at `supported template types` section.
         *
         * Recommended default usage to return a vector of bytes:
         * `std::vector<std::byte> bytes = some_char->read<std::vector<std::byte>>();`
         *
         * You can also return an int if you know your characteristic value is a 4-byte integer in little-endian order:
         * `int x = some_char->read<int>();`
         * @tparam T the type your want the data to be read as.
         *
         * @return a byte vector.
         */
        template<typename T>
        T read();


        /**
         * Write to characteristic using a vector of bytes. Will not print error if write fails.
         * You can transmit more data with this method than write_with_response().
         * Compatible template types are listed at `supported template types` section.
         *
         * examples:
         * `std::vector<std::byte> data = {...};`
         * `some_char->write_without_response<std::byte>(data);`
         * `some_char->write_without_response<int>(1);`
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
         * Compatible template types are listed at `supported template types` section.
         *
         * examples:
         * `std::vector<std::byte> data = {...};`
         * `some_char->write_with_response<std::byte>(data);`
         * `some_char->write_with_response<int>(1);`
         *
         * @param data byte vector of your data.
         */
        template<typename T>
        void write_with_response(T data);

        template<typename T>
        void write_with_response(const std::vector<T> &data);


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

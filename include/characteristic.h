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
         * Read the characteristic. Up to you how to read the char array.
         * @return a byte array.
         */
        uint8_t *read();

        /**
         * Write to characteristic. Performs this action asynchronously and does not
         * give any indication of write_without_response failure. Can write_without_response more data to your device than if you used
         * write_with_response.
         * @param data the data you want to write.
         * @param length how many elements in the data array (not to be confused with the total size)
         */
        void write_without_response(uint8_t *data, int length);

        /**
         * Write to characteristic with response. Blocks calling thread until write_without_response is confirmed to
         * be either success or failure. If the write_without_response fails, you will be alerted.
         * @param data the data you want to write.
         * @param length how many elements in the data array (not to be confused with the total size)
         */
        void write_with_response(uint8_t *data, int length);

        /**
         * Enable notifications from the characteristic and set callback function to do something
         * with the data when the device notifies.
         * @param callback function to do something with notificaiton data.
         */
        void notify(const std::function<void(uint8_t *)> &callback);


    private:
        struct CharacteristicImpl;
        CharacteristicImpl *cImpl;
    };
}

#endif //FEELING_BLUE_CHARACTERISTIC_H

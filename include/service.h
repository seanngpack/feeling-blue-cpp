#ifndef FEELING_BLUE_SERVICE_H
#define FEELING_BLUE_SERVICE_H


#include <string>

namespace bluetooth {

    namespace detail {
        namespace wrapper {
            class Wrapper;
        }
    }

    class Characteristic;

    /**
     * Represents a service.
     */
    class Service {
    public:

        Service(const std::string &service_uuid, std::shared_ptr<detail::wrapper::Wrapper> bt);

        ~Service();

        /**
         * Scan the device for given advertised characteristic. Return the characteristic if found, otherwise returns a nullptr.
         *
         * @param uuid uuid of the characteristic.
         * @return the characteristic or nullptr.
         */
        std::shared_ptr<Characteristic> find_characteristic(const std::string &uuid);

        /**
         * Retrieved connected characteristic. If the characteristic isn't discovered and connected yet, then return nullptr.
         *
         * @param char_uuid uuid of characteristic.
         * @return the characteristic or nullptr.
         */
        std::shared_ptr<Characteristic> characteristic(const std::string &uuid);

        std::string uuid();


    private:
        struct ServiceImpl;
        ServiceImpl *sImpl;
    };
}

#endif //FEELING_BLUE_SERVICE_H

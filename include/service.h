#ifndef FEELING_BLUE_SERVICE_H
#define FEELING_BLUE_SERVICE_H


#include <string>

namespace bluetooth {

    namespace wrapper {
        class Wrapper;
    }

    namespace handler {
        class EventHandler;
    }

    class Characteristic;

    /**
     * Represents a uuid service.
     */
    class Service {
    public:

        Service(const std::string &service_uuid, std::shared_ptr<handler::EventHandler> event_handler);

        ~Service();

        std::shared_ptr<Characteristic> find_characteristic(const std::string &uuid);

        /**
         * The service stores connected characteristics into a list. This method retrieves the characteristic
         * matching the given uuid.
         * @param char_uuid uuid of characteristic.
         * @return the characteristic.
         */
        std::shared_ptr<Characteristic> get_characteristic(const std::string &uuid);

        std::string uuid();


    private:
        struct ServiceImpl;
        ServiceImpl *sImpl;
    };
}

#endif //FEELING_BLUE_SERVICE_H

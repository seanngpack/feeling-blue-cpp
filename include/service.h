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
     * Owned by peripheral.
     */
    class Service {
    public:

        Service(const std::string &uuid, handler::EventHandler *event_handler);

        ~Service();

        std::shared_ptr<Characteristic> find_characteristic(const std::string &char_uuid);


    private:
        struct ServiceImpl;
        ServiceImpl *sImpl;
    };
}

#endif //FEELING_BLUE_SERVICE_H

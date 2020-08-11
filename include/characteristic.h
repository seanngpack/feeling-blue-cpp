#ifndef FEELING_BLUE_CHARACTERISTIC_H
#define FEELING_BLUE_CHARACTERISTIC_H

#include <string>

namespace bluetooth {

    namespace wrapper {
        class Wrapper;
    }

    namespace handler {
        class EventHandler;
    }

    class Service;

    /**
     * Represents a characteristic.
     * Owned by service.
     */
    class Characteristic {
    public:

        Characteristic(const std::string &uuid,
                       const std::string &service_uuid,
                       handler::EventHandler *event_handler);

        ~Characteristic();


    private:
        struct CharacteristicImpl;
        CharacteristicImpl *cImpl;
    };
}

#endif //FEELING_BLUE_CHARACTERISTIC_H

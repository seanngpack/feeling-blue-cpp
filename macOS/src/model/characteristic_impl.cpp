#include "characteristic.h"
#include "service.h"
#include <string>
#include <utility>
#include <vector>

namespace bluetooth {

    struct Characteristic::CharacteristicImpl {
    public:
        CharacteristicImpl(const std::string &uuid,
                           const std::string &service_uuid,
                           handler::EventHandler *event_handler) :
                uuid(uuid), service_uuid(service_uuid), event_handler(event_handler) {}

        ~CharacteristicImpl() {

        }

    private:
        std::string uuid;
        std::string service_uuid;
        handler::EventHandler *event_handler;
    };

    Characteristic::Characteristic(const std::string &uuid,
                                   const std::string &service_uuid,
                                   handler::EventHandler *event_handler) :
            cImpl(new CharacteristicImpl(uuid, service_uuid, event_handler)) {}

    Characteristic::~Characteristic() {
        delete cImpl;
    }
}
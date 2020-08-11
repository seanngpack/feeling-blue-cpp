#include "characteristic.h"
#include "service.h"
#include <string>
#include <utility>
#include <vector>

namespace bluetooth {

    struct Characteristic::CharacteristicImpl {
    public:
        CharacteristicImpl(const std::string &char_uuid,
                           const std::string &service_uuid,
                           std::shared_ptr<handler::EventHandler> event_handler) :
                char_uuid(char_uuid), service_uuid(service_uuid), event_handler(std::move(event_handler)) {}

        ~CharacteristicImpl() {

        }

        std::string uuid() {
            return char_uuid;
        }

    private:
        std::string char_uuid;
        std::string service_uuid;
        std::shared_ptr<handler::EventHandler> event_handler;
    };

    Characteristic::Characteristic(const std::string &char_uuid,

                                   const std::string &service_uuid,
                                   std::shared_ptr<handler::EventHandler> event_handler) :
            cImpl(new CharacteristicImpl(char_uuid, service_uuid, std::move(event_handler))) {}

    Characteristic::~Characteristic() {
        delete cImpl;
    }

    std::string Characteristic::uuid() {
        return cImpl->uuid();
    }
}
#include "characteristic.h"
#include "wrapper.h"
#include <string>
#include <utility>

namespace bluetooth {

    struct Characteristic::CharacteristicImpl {
    public:
        CharacteristicImpl(const std::string &char_uuid,
                           const std::string &service_uuid,
                           std::shared_ptr<wrapper::Wrapper> bt) :
                char_uuid(char_uuid), service_uuid(service_uuid), bt(std::move(bt)) {}

        ~CharacteristicImpl() {

        }

        std::string uuid() {
            return char_uuid;
        }

        uint8_t *read() {
            return bt->read(service_uuid, char_uuid);
        }

    private:
        std::string char_uuid;
        std::string service_uuid;
        std::shared_ptr<wrapper::Wrapper> bt;
    };

    Characteristic::Characteristic(const std::string &char_uuid,
                                   const std::string &service_uuid,
                                   std::shared_ptr<wrapper::Wrapper> bt) :
            cImpl(new CharacteristicImpl(char_uuid, service_uuid, std::move(bt))) {}

    Characteristic::~Characteristic() {
        delete cImpl;
    }

    std::string Characteristic::uuid() {
        return cImpl->uuid();
    }

    uint8_t *Characteristic::read() {
        return cImpl->read();
    }
}
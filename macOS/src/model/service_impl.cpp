#include "service.h"
#include "characteristic.h"
#include "event_handler.h"
#include <vector>

namespace bluetooth {

    struct Service::ServiceImpl {
    public:
        ServiceImpl(const std::string &uuid, handler::EventHandler *event_handler)
                :
                uuid(uuid), event_handler(event_handler) {}

        std::shared_ptr<Characteristic> find_characteristic(const std::string &char_uuid) {
            std::shared_ptr<Characteristic> c = event_handler->find_characteristic(char_uuid, uuid);
            characteristics.push_back(c);
            return c;
        }

        ~ServiceImpl() {

        }

    private:
        std::string uuid;
        std::vector<std::shared_ptr<Characteristic>> characteristics;
        handler::EventHandler *event_handler;
    };

    // Wrapper implementation //

    Service::Service(const std::string &uuid,
                     handler::EventHandler *event_handler) :
            sImpl(new ServiceImpl(uuid, event_handler)) {}

    Service::~Service() {
        delete sImpl;
    }

    std::shared_ptr<Characteristic> Service::find_characteristic(const std::string &uuid) {
        return sImpl->find_characteristic(uuid);
    }


}
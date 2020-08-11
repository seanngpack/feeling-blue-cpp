#include "service.h"

namespace bluetooth {

    struct Service::ServiceImpl {
    public:
        ServiceImpl(const std::string &uuid, std::shared_ptr<wrapper::Wrapper> bt, handler::EventHandler *event_handler)
                :
                uuid(uuid), bt(std::move(bt)), event_handler(event_handler) {}


    private:
        std::shared_ptr<wrapper::Wrapper> bt;
        std::string uuid;
        handler::EventHandler *event_handler;
    };

    // Wrapper implementation //

    Service::Service(const std::string &uuid,
                     std::shared_ptr<wrapper::Wrapper> bt,
                     handler::EventHandler *event_handler) :
            sImpl(new ServiceImpl(uuid, std::move(bt), event_handler)) {}

    Service::~Service() {
        delete sImpl;
    }


}
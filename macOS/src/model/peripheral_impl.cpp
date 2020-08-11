#include "peripheral.h"
#include "service.h"
#include "event_handler.h"
#include <string>
#include <utility>
#include <vector>

namespace bluetooth {

    struct Peripheral::PeripheralImpl {
    public:
        PeripheralImpl(const std::string &name,
                       std::shared_ptr<wrapper::Wrapper> bt,
                       handler::EventHandler *event_handler) :
                name(name), bt(std::move(bt)), event_handler(event_handler) {}

        Service find_service(const std::string &uuid) {
            Service *s = event_handler->find_service(uuid);
            services.push_back(*s);
            return *s;
        }

    private:
        std::shared_ptr<wrapper::Wrapper> bt;
        std::string name;
        std::vector<Service> services;
        handler::EventHandler *event_handler;
    };

    Peripheral::Peripheral(const std::string &name,
                           std::shared_ptr<wrapper::Wrapper> bt,
                           handler::EventHandler *event_handler) :
            pImpl(new PeripheralImpl(name, std::move(bt), event_handler)) {}

    Peripheral::~Peripheral() {
        delete pImpl;
    }

    Service Peripheral::find_service(const std::string &uuid) {
        return pImpl->find_service(uuid);
    }
}
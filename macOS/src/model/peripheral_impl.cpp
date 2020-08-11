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
                       handler::EventHandler *event_handler) :
                name(name), event_handler(event_handler) {}

        ~PeripheralImpl() {
            for (int i = 0; i < services.size(); i++) {
                delete services[i];
            }
        }

        Service find_service(const std::string &uuid) {
            Service *s = event_handler->find_service(uuid);
            services.push_back(s);
            return *s;
        }

    private:
        std::string name;
        std::vector<Service *> services;
        handler::EventHandler *event_handler;
    };

    Peripheral::Peripheral(const std::string &name,
                           handler::EventHandler *event_handler) :
            pImpl(new PeripheralImpl(name, event_handler)) {}

    Peripheral::~Peripheral() {
        delete pImpl;
    }

    Service Peripheral::find_service(const std::string &uuid) {
        return pImpl->find_service(uuid);
    }
}
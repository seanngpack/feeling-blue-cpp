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
                       std::shared_ptr<handler::EventHandler> event_handler) :
                name(name), event_handler(std::move(event_handler)) {}

        ~PeripheralImpl() {

        }

        std::shared_ptr<Service> find_service(const std::string &uuid) {
            std::shared_ptr<Service> s = event_handler->find_service(uuid);
            services.push_back(s);
            return s;
        }

    private:
        std::string name;
        std::vector<std::shared_ptr<Service>> services;
        std::shared_ptr<handler::EventHandler> event_handler;
    };

    Peripheral::Peripheral(const std::string &name,
                           std::shared_ptr<handler::EventHandler> event_handler) :
            pImpl(new PeripheralImpl(name, std::move(event_handler))) {}

    Peripheral::~Peripheral() {
        delete pImpl;
    }

    std::shared_ptr<Service> Peripheral::find_service(const std::string &uuid) {
        return pImpl->find_service(uuid);
    }
}
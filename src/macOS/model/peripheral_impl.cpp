#include "peripheral.h"
#include "service.h"
#include "wrapper.h"
#include <string>
#include <utility>
#include <vector>

namespace bluetooth {

    struct Peripheral::PeripheralImpl {
    public:
        PeripheralImpl(std::string peripheral_name,
                       std::shared_ptr<detail::wrapper::Wrapper> bt) :
                peripheral_name(std::move(peripheral_name)), bt(std::move(bt)) {}

        ~PeripheralImpl() = default;

        std::shared_ptr<Service> find_service(const std::string &service_uuid) {
            if (bt->find_service(service_uuid)) {
                std::shared_ptr<bluetooth::Service> s = std::make_shared<bluetooth::Service>(service_uuid, bt);
                services.push_back(s);
                return s;
            }
            return nullptr;
        }

        std::shared_ptr<Service> service(const std::string &uuid) {
            for (auto s : services) {
                if (uuid == s->uuid()) {
                    return s;
                }
            }
            return nullptr;
        }

        std::string name() {
            return peripheral_name;
        }

        void disconnect() {
            bt->disconnect();
        }

    private:
        std::string peripheral_name;
        std::vector<std::shared_ptr<Service>> services;
        std::shared_ptr<detail::wrapper::Wrapper> bt;
    };

    Peripheral::Peripheral(const std::string &name,
                           std::shared_ptr<detail::wrapper::Wrapper> bt) :
            pImpl(new PeripheralImpl(name, std::move(bt))) {}

    Peripheral::~Peripheral() {
        delete pImpl;
    }

    std::shared_ptr<Service> Peripheral::find_service(const std::string &service_uuid) {
        return pImpl->find_service(service_uuid);
    }

    std::shared_ptr<Service> Peripheral::service(const std::string &service_uuid) {
        return pImpl->service(service_uuid);
    }

    std::string Peripheral::name() {
        return pImpl->name();
    }

    void Peripheral::disconnect() {
        pImpl->disconnect();
    }
}
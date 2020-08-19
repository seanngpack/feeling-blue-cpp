#include "peripheral.h"
#include "service.h"
#include "wrapper.h"
#include <string>
#include <utility>
#include <vector>

namespace bluetooth {

    struct Peripheral::PeripheralImpl {
    public:
        PeripheralImpl(std::string name,
                       std::shared_ptr<wrapper::Wrapper> bt) :
                name(std::move(name)), bt(std::move(bt)) {}

        ~PeripheralImpl() {

        }

        std::shared_ptr<Service> find_service(const std::string &service_uuid) {
            if (bt->find_service(service_uuid)) {
                std::shared_ptr<bluetooth::Service> s = std::make_shared<bluetooth::Service>(service_uuid, bt);
                services.push_back(s);
                return s;
            }
            return nullptr;
        }

        std::shared_ptr<Service> get_service(const std::string &uuid) {
            for (auto s : services) {
                if (uuid == s->uuid()) {
                    return s;
                }
            }
            return nullptr;
        }

        void set_name(const std::string &n) {
            name = n;
        }

        std::string get_name() {
            return name;
        }

        void disconnect() {
            bt->disconnect();
        }

    private:
        std::string name;
        std::vector<std::shared_ptr<Service>> services;
        std::shared_ptr<wrapper::Wrapper> bt;
    };

    Peripheral::Peripheral(const std::string &name,
                           std::shared_ptr<wrapper::Wrapper> bt) :
            pImpl(new PeripheralImpl(name, std::move(bt))) {}

    Peripheral::~Peripheral() {
        delete pImpl;
    }

    std::shared_ptr<Service> Peripheral::find_service(const std::string &service_uuid) {
        return pImpl->find_service(service_uuid);
    }

    std::shared_ptr<Service> Peripheral::get_service(const std::string &service_uuid) {
        return pImpl->get_service(service_uuid);
    }

    void Peripheral::set_name(const std::string &n) {
        pImpl->set_name(n);
    }

    std::string Peripheral::get_name() {
        return pImpl->get_name();
    }

    void Peripheral::disconnect() {
        pImpl->disconnect();
    }
}
#include "peripheral.h"
#include "service.h"
#include "wrapper.h"
#include <string>
#include <utility>
#include <vector>

namespace bluetooth {

    struct Peripheral::PeripheralImpl {
    public:
        PeripheralImpl(const std::string &name,
                       std::shared_ptr<wrapper::Wrapper> bt) :
                name(name), bt(std::move(bt)) {}

        ~PeripheralImpl() {

        }

        std::shared_ptr<Service> find_service(const std::string &uuid) {
            std::shared_ptr<Service> s = bt->find_service(uuid);
            services.push_back(s);
            return s;
        }

        std::shared_ptr<Service> get_service(const std::string &uuid) {
            for (auto s : services) {
                if (uuid == s->uuid()) {
                    return s;
                }
            }
            return nullptr;
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

    std::shared_ptr<Service> Peripheral::find_service(const std::string &uuid) {
        return pImpl->find_service(uuid);
    }

    std::shared_ptr<Service> Peripheral::get_service(const std::string &uuid) {
        return pImpl->get_service(uuid);
    }
}
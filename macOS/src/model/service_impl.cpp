#include "service.h"
#include "characteristic.h"
#include "wrapper.h"
#include <utility>
#include <vector>

namespace bluetooth {

    struct Service::ServiceImpl {
    public:
        ServiceImpl(std::string service_uuid, std::shared_ptr<wrapper::Wrapper> bt)
                : service_uuid(std::move(service_uuid)), bt(std::move(bt)) {}


        ~ServiceImpl() = default;

        std::shared_ptr<Characteristic> find_characteristic(const std::string &uuid) {
            if (bt->find_characteristic(uuid, service_uuid)) {
                std::shared_ptr<bluetooth::Characteristic> c = std::make_shared<bluetooth::Characteristic>(uuid,
                                                                                                           service_uuid,
                                                                                                           bt);
                characteristics.push_back(c);
                return c;
            }
            return nullptr;
        }

        std::shared_ptr<Characteristic> characteristic(const std::string &uuid) {
            for (auto c : characteristics) {
                if (uuid == c->uuid()) {
                    return c;
                }
            }
            return nullptr;
        }

        std::string uuid() {
            return service_uuid;
        }


    private:
        std::string service_uuid;
        std::vector<std::shared_ptr<Characteristic>> characteristics;
        std::shared_ptr<wrapper::Wrapper> bt;
    };

    Service::Service(const std::string &uuid,
                     std::shared_ptr<wrapper::Wrapper> bt) :
            sImpl(new ServiceImpl(uuid, std::move(bt))) {}

    Service::~Service() {
        delete sImpl;
    }

    std::shared_ptr<Characteristic> Service::find_characteristic(const std::string &uuid) {
        return sImpl->find_characteristic(uuid);
    }

    std::shared_ptr<Characteristic> Service::characteristic(const std::string &uuid) {
        return sImpl->characteristic(uuid);
    }

    std::string Service::uuid() {
        return sImpl->uuid();
    }
}
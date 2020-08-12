#include "service.h"
#include "characteristic.h"
#include "wrapper.h"
#include <utility>
#include <vector>

namespace bluetooth {

    struct Service::ServiceImpl {
    public:
        ServiceImpl(const std::string &service_uuid, std::shared_ptr<wrapper::Wrapper> bt)
                : service_uuid(service_uuid), bt(bt) {}


        ~ServiceImpl() {

        }

        std::shared_ptr<Characteristic> find_characteristic(const std::string &uuid) {
            std::shared_ptr<Characteristic> c = bt->find_characteristic(uuid, service_uuid);
            characteristics.push_back(c);
            return c;
        }

        std::shared_ptr<Characteristic> get_characteristic(const std::string &uuid) {
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

    std::shared_ptr<Characteristic> Service::get_characteristic(const std::string &uuid) {
        return sImpl->find_characteristic(uuid);
    }

    std::string Service::uuid() {
        return sImpl->uuid();
    }
}
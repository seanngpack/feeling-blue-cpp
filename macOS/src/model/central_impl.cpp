#include "central.h"
#include "wrapper.h"
#include "peripheral.h"
#include <string>

namespace bluetooth {


    struct Central::CentralImpl {
    public:
        CentralImpl()
                : bt(std::make_shared<wrapper::Wrapper>()) {
        }

        ~CentralImpl() {

        }

        void start_bluetooth() {
            bt->start_bluetooth();
        }

        std::shared_ptr<Peripheral> find_peripheral(const std::vector<std::string> &uuids) {
            if (bt->find_peripheral(uuids)) {
                return std::make_shared<bluetooth::Peripheral>(bt->get_peripheral_name(), bt);
            }
            return nullptr;
        }

        std::shared_ptr<Peripheral> find_peripheral(const std::string &name) {
            if (bt->find_peripheral(name)) {
                return std::make_shared<bluetooth::Peripheral>(bt->get_peripheral_name(), bt);
            }
            return nullptr;
        }

    private:
        std::shared_ptr<wrapper::Wrapper> bt;
        std::shared_ptr<Peripheral> peripheral;
    };

    // Central implementation //

    Central::Central() : cImpl(new CentralImpl()) {}

    Central::~Central() {
        delete cImpl;
    }

    void Central::start_bluetooth() {
        cImpl->start_bluetooth();
    }

    std::shared_ptr<Peripheral> Central::find_peripheral(const std::vector<std::string> &uuids) {
        return cImpl->find_peripheral(uuids);
    }

    std::shared_ptr<Peripheral> Central::find_peripheral(const std::string &name) {
        return cImpl->find_peripheral(name);
    }

}
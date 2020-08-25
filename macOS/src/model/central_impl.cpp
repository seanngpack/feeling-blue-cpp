#include "central.h"
#include "wrapper.h"
#include "peripheral.h"
#include <string>
#include "spdlog/spdlog.h"

namespace bluetooth {


    struct Central::CentralImpl {
    public:
        CentralImpl()
                : bt(std::make_shared<detail::wrapper::Wrapper>()) {
            spdlog::set_level(spdlog::level::debug);
            spdlog::set_pattern("[feeling-blue] [%^%l%$] %v");
        }

        ~CentralImpl() = default;

        void start_bluetooth() {
            bt->start_bluetooth();
        }

        std::shared_ptr<Peripheral> find_peripheral(const std::vector<std::string> &uuids) {
            if (bt->find_peripheral(uuids)) {
                periph = std::make_shared<bluetooth::Peripheral>(bt->get_peripheral_name(), bt);
                return periph;
            }
            return nullptr;
        }

        std::shared_ptr<Peripheral> find_peripheral(const std::string &name) {
            if (bt->find_peripheral(name)) {
                periph = std::make_shared<bluetooth::Peripheral>(bt->get_peripheral_name(), bt);
                return periph;
            }
            return nullptr;
        }

        std::shared_ptr<Peripheral> peripheral(const std::string &name) {
            return periph;
        }

    private:
        std::shared_ptr<detail::wrapper::Wrapper> bt;
        std::shared_ptr<Peripheral> periph;
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

    std::shared_ptr<Peripheral> Central::peripheral(const std::string &uuid) {
        return cImpl->peripheral(uuid);
    }

}
#include "central.h"
#include "wrapper.h"
#include "peripheral.h"
#include "event_handler.h"
#include <string>

namespace bluetooth {


    struct Central::CentralImpl {
    public:
        CentralImpl() :
                event_handler(std::make_shared<handler::EventHandler>()),
                bluetooth_object(std::make_shared<wrapper::Wrapper>()) {
            // TODO: same with other constructors, check for nullptr
        }

        ~CentralImpl() {

        }

        void start_bluetooth() {
            event_handler->start_bluetooth();
        }

        std::shared_ptr<Peripheral> find_peripheral(const std::vector<std::string> &uuids) {
            peripheral = event_handler->find_peripheral(uuids);
            return peripheral;
        }

        std::shared_ptr<Peripheral> find_peripheral(const std::string &name) {
            peripheral = event_handler->find_peripheral(name);
            return peripheral;
        }

    private:
        std::shared_ptr<handler::EventHandler> event_handler;
        std::shared_ptr<wrapper::Wrapper> bluetooth_object;
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
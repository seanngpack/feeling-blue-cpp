#include "central.h"
#include "wrapper.h"
#include "peripheral.h"
#include "event_handler.h"
#include <string>

namespace bluetooth {


    struct Central::CentralImpl {
    public:
        CentralImpl() :
                bluetooth_object(std::make_shared<wrapper::Wrapper>()),
                event_handler(std::make_shared<handler::EventHandler>()) {
            // TODO: same with other constructors, check for nullptr
        }

        ~CentralImpl() {
            delete peripheral;
        }

        void start_bluetooth() {
            event_handler->start_bluetooth();
        }

        std::shared_ptr<Peripheral> find_peripheral(const std::vector<std::string> &uuids) {
            return event_handler->find_peripheral(uuids);
        }

        std::shared_ptr<Peripheral> find_peripheral(const std::string &name) {
            return event_handler->find_peripheral(name);
        }

    private:
        std::shared_ptr<wrapper::Wrapper> bluetooth_object;
        std::shared_ptr<handler::EventHandler> event_handler;
        Peripheral *peripheral;
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
#include "central.h"
#include "wrapper.h"
#include "peripheral.h"
#include "central_event_handler.h"
#include <string>

namespace bluetooth {


    struct Central::CentralImpl {
    public:
        CentralImpl() :
                bluetooth_object(std::make_shared<wrapper::Wrapper>()),
                event_handler(new handler::CentralEventHandler(std::move(bluetooth_object))) {}

        ~CentralImpl() {
            delete event_handler;
            delete peripheral;
        }

        void start_bluetooth() {
            event_handler->start_bluetooth();
        }

        Peripheral find_peripheral(const std::vector<std::string> &uuids) {
            peripheral = event_handler->find_peripheral(uuids);
            return *peripheral;
        }

        Peripheral find_peripheral(const std::string &name) {
            peripheral = event_handler->find_peripheral(name);
            return *peripheral;
        }

    private:
        std::shared_ptr<wrapper::Wrapper> bluetooth_object;
        handler::CentralEventHandler *event_handler;
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

    Peripheral Central::find_peripheral(const std::vector<std::string> &uuids) {
        return cImpl->find_peripheral(uuids);
    }

    Peripheral Central::find_peripheral(const std::string &name) {
        return cImpl->find_peripheral(name);
    }

}
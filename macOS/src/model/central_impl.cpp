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
        }

        void start_bluetooth() {
            bluetooth_object->start_bluetooth();
        }


        Peripheral find_peripheral(const std::vector<std::string> &uuids) {
            return event_handler->find_peripheral(uuids);
        }

        Peripheral find_peripheral(const std::string &name) {
            return event_handler->find_peripheral(name);
        }

    private:
        std::shared_ptr<wrapper::Wrapper> bluetooth_object;
        handler::CentralEventHandler *event_handler;
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
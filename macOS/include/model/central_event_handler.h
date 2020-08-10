#ifndef FEELING_BLUE_CENTRAL_EVENT_HANDLER_H
#define FEELING_BLUE_CENTRAL_EVENT_HANDLER_H

#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <functional>
#include "wrapper.h"


namespace bluetooth {
    class Peripheral;
}

namespace handler {

    // TODO: Reduce number of mutexes and conditional variables to just one.
    // mux & ready

    class CentralEventHandler {
    public:

        CentralEventHandler(std::shared_ptr<bluetooth::wrapper::Wrapper> bluetooth);

        void rotate_by(int degs);

        void start_bluetooth();

        bluetooth::Peripheral find_peripheral(const std::vector<std::string> &uuids);

        bluetooth::Peripheral find_peripheral(const std::string &name);

        void set_is_powered_on(bool connected);

        void set_is_peripheral_found(bool found);

        ~CentralEventHandler();

        std::mutex power_mutex;
        std::mutex peripheral_mutex;
        std::condition_variable power_cv;
        std::condition_variable peripheral_cv;

    private:
        std::shared_ptr<bluetooth::wrapper::Wrapper> bluetooth_object;
        bool is_powered_on;
        bool is_peripheral_found;
    };
}


#endif //FEELING_BLUE_CENTRAL_EVENT_HANDLER_H

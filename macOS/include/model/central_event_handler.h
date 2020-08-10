#ifndef FEELING_BLUE_CENTRAL_EVENT_HANDLER_H
#define FEELING_BLUE_CENTRAL_EVENT_HANDLER_H

#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <functional>


namespace bluetooth {
    class Peripheral;
}

namespace bluetooth {
    namespace wrapper {
        class Wrapper;
    }

    namespace handler {

        // TODO: Reduce number of mutexes and conditional variables to just one.
        // mux & ready

        class CentralEventHandler {
        public:

            CentralEventHandler(std::shared_ptr<wrapper::Wrapper> bluetooth);

            void rotate_by(int degs);

            void start_bluetooth();

            Peripheral *find_peripheral(const std::vector<std::string> &uuids);

            Peripheral *find_peripheral(const std::string &name);

            void set_proceed(bool connected);

            ~CentralEventHandler();

            std::mutex central_mutex;
            std::condition_variable cv;

        private:
            std::shared_ptr<bluetooth::wrapper::Wrapper> bluetooth_object;
            bool proceed;
        };
    }
}

#endif //FEELING_BLUE_CENTRAL_EVENT_HANDLER_H

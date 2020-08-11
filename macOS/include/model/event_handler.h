#ifndef FEELING_BLUE_EVENT_HANDLER_H
#define FEELING_BLUE_EVENT_HANDLER_H

#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <functional>


namespace bluetooth {
    namespace wrapper {
        class Wrapper;
    }

    class Peripheral;

    class Service;

    namespace handler {

        class EventHandler {
        public:

            EventHandler(std::shared_ptr<wrapper::Wrapper> bluetooth);

            void rotate_by(int degs);

            void start_bluetooth();

            Service *find_service(const std::string &uuid);

            Peripheral *find_peripheral(const std::vector<std::string> &uuids);

            Peripheral *find_peripheral(const std::string &name);

            void set_proceed(bool connected);

            ~EventHandler();

            std::mutex mut;
            std::condition_variable cv;
            bool service_found;

        private:
            std::shared_ptr<bluetooth::wrapper::Wrapper> bluetooth_object;
            bool proceed;
        };
    }
}

#endif //FEELING_BLUE_EVENT_HANDLER_H

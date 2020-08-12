#ifndef FEELING_BLUE_EVENT_HANDLER_H
#define FEELING_BLUE_EVENT_HANDLER_H

#include <condition_variable>
#include <future>
#include <memory>
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

    class Characteristic;

    namespace handler {

        class EventHandler : public std::enable_shared_from_this<EventHandler> {
        public:

            EventHandler();

            ~EventHandler();

            void rotate_by(int degs);

            void start_bluetooth();

            std::shared_ptr<Service> find_service(const std::string &uuid);

            std::shared_ptr<Peripheral> find_peripheral(const std::vector<std::string> &uuids);

            std::shared_ptr<Peripheral> find_peripheral(const std::string &name);

            std::shared_ptr<Characteristic>
            find_characteristic(const std::string &char_uuid, const std::string &service_uuid);

            uint8_t *read(const std::string &service_uuid, const std::string &char_uuid);

            void set_proceed(bool connected);


            std::mutex mut;
            std::condition_variable cv;
            bool service_found;
            bool char_found;

        private:
            std::unique_ptr<bluetooth::wrapper::Wrapper> bluetooth_object;
            bool proceed;
        };
    }
}

#endif //FEELING_BLUE_EVENT_HANDLER_H

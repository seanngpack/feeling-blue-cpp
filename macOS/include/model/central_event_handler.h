#ifndef FEELING_BLUE_CENTRAL_EVENT_HANDLER_H
#define FEELING_BLUE_CENTRAL_EVENT_HANDLER_H

#include "wrapper.h"
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <functional>

namespace handler {

    class CentralEventHandler {
    public:


        CentralEventHandler(std::shared_ptr<wrapper::Wrapper> bluetooth);

        void rotate_by(int degs);

        void start_bluetooth();

        void set_is_bt_connected(bool is_connected);

        void set_is_table_rotating(bool is_rotating);


        ~CentralEventHandler();

        std::mutex bt_mutex;
        std::mutex table_mutex;
        std::condition_variable bt_cv;
        std::condition_variable table_cv;

    private:
        std::shared_ptr<wrapper::Wrapper> bluetooth;
        bool is_bt_connected;
        bool is_table_rotating;


    };
}


#endif //FEELING_BLUE_CENTRAL_EVENT_HANDLER_H

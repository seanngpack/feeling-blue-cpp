#ifndef FEELING_BLUE_CENTRALEVENTHANDLER_H
#define FEELING_BLUE_CENTRALEVENTHANDLER_H

#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <functional>

namespace handler {

    class CentralEventHandler {
    public:

        /**
         * On initialization, will search for settings.json and set the current position
         * from that file.
         * Might be bad design because arduino is kinda taking responsibility from the
         * filehandler class.
         */
        CentralEventHandler();

        void rotate_by(int degs);


        void connect_bluetooth();

        void set_is_bt_connected(bool is_connected);

        void set_is_table_rotating(bool is_rotating);


        ~CentralEventHandler();

        std::mutex bt_mutex;
        std::mutex table_mutex;
        std::condition_variable bt_cv;
        std::condition_variable table_cv;

    private:
        void *bluetooth_object;
        bool is_bt_connected;
        bool is_table_rotating;


    };
}


#endif //FEELING_BLUE_CENTRALEVENTHANDLER_H

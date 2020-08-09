#include "wrapper.h"
#include "central_event_handler.h"

handler::CentralEventHandler::CentralEventHandler(std::shared_ptr<wrapper::Wrapper> bluetooth) :
        bluetooth(std::move(bluetooth)),
        is_bt_connected(false),
        is_table_rotating(false) {
    bluetooth->set_handler(this);
}

void handler::CentralEventHandler::start_bluetooth() {
    std::unique_lock<std::mutex> ul(bt_mutex);
    bluetooth->start_bluetooth();
    bt_cv.wait(ul, [this]() { return is_bt_connected; });
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(1s); // needs a hard delay to prevent bluetooth overloading
    std::cout << "Finally connected to bluetooth, unblocking thread" << std::endl;
}

void handler::CentralEventHandler::rotate_by(int degs) {
    using namespace std::literals::chrono_literals;
    std::unique_lock<std::mutex> ul(table_mutex);
//    rotate(bluetooth, degs);

    table_cv.wait(ul, [this]() { return !is_table_rotating; });
    std::this_thread::sleep_for(.5s);
}


void handler::CentralEventHandler::set_is_bt_connected(bool is_connected) {
    is_bt_connected = is_connected;
}

void handler::CentralEventHandler::set_is_table_rotating(bool is_rotating) {
    is_table_rotating = is_rotating;
}

handler::CentralEventHandler::~CentralEventHandler() {
    bluetooth = nullptr;
}




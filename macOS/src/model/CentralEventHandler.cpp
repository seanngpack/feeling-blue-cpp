#include "CoreBluetoothWrapper.h"
#include "CentralEventHandler.h"

handler::CentralEventHandler::CentralEventHandler() :
        bluetooth_object(get_bluetooth_obj()),
        is_bt_connected(false),
        is_table_rotating(false) {
    set_handler(this, bluetooth_object);
}

void handler::CentralEventHandler::connect_bluetooth() {
    std::unique_lock<std::mutex> ul(bt_mutex);
    start_bluetooth(bluetooth_object);
    bt_cv.wait(ul, [this]() { return is_bt_connected; });
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(1s); // needs a hard delay to prevent bluetooth overloading
    std::cout << "Finally connected to bluetooth, unblocking thread" << std::endl;
}

void handler::CentralEventHandler::rotate_by(int degs) {
    using namespace std::literals::chrono_literals;
    std::unique_lock<std::mutex> ul(table_mutex);
    rotate(bluetooth_object, degs);
    if (degs < 0) {
        degs += 360;
    }

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
    bluetooth_object = nullptr;
}




#include "peripheral.h"
#include <string>
#include <utility>

namespace bluetooth {

    struct Peripheral::PeripheralImpl {
    public:
        PeripheralImpl(const std::string &name, std::shared_ptr<wrapper::Wrapper> bt) :
                name(name), bt(std::move(bt)) {}


    private:
        std::shared_ptr<wrapper::Wrapper> bt;
        std::string name;
    };

    Peripheral::Peripheral(const std::string &name, std::shared_ptr<wrapper::Wrapper> bt) :
            pImpl(new PeripheralImpl(name, bt)) {}

    Peripheral::~Peripheral() {
        delete pImpl;
    }

    // Peripheral implementation //



}
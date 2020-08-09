#include "peripheral.h"
#include <string>

namespace bluetooth {

    struct Peripheral::PeripheralImpl {
    public:
        PeripheralImpl() {}

        void set_name(const std::string &name) {
            this->name = name;
        }

        void set_bluetooth(std::shared_ptr<Wrapper> bt) {
            this->bt = std::move(bt);
        }

    private:
        std::shared_ptr<Wrapper> bt;
        std::string name;
    };

    Peripheral::Peripheral() : pImpl(new PeripheralImpl()) {}

    Peripheral::~Peripheral() {
        delete pImpl;
    }

    // Peripheral implementation //

    void Peripheral::set_name(const std::string &name) {
        pImpl->set_name(name);
    }

    void Peripheral::set_bluetooth(std::shared_ptr<Wrapper> bt) {
        pImpl->set_bluetooth(std::move(bt));
    }
}
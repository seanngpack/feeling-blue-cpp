#include "characteristic.h"
#include "wrapper.h"
#include <string>
#include <utility>

namespace bluetooth {

    struct Characteristic::CharacteristicImpl {
    public:
        CharacteristicImpl(const std::string &char_uuid,
                           const std::string &service_uuid,
                           std::shared_ptr<wrapper::Wrapper> bt) :
                char_uuid(char_uuid), service_uuid(service_uuid), bt(std::move(bt)) {}

        ~CharacteristicImpl() {

        }

        std::string uuid() {
            return char_uuid;
        }

        uint8_t *read() {
            return bt->read(service_uuid, char_uuid);
        }

        void write_without_response(uint8_t *data, int length) {
            bt->write_without_response(data, length, service_uuid, char_uuid);
        }

        void write_with_response(uint8_t *data, int length) {
            bt->write_with_response(data, length, service_uuid, char_uuid);
        }

        void notify(std::function<void(uint8_t *)> callback) {
            bt->notify(service_uuid, char_uuid, callback);
        }

    private:
        std::string char_uuid;
        std::string service_uuid;
        std::shared_ptr<wrapper::Wrapper> bt;
    };

    Characteristic::Characteristic(const std::string &char_uuid,
                                   const std::string &service_uuid,
                                   std::shared_ptr<wrapper::Wrapper> bt) :
            cImpl(new CharacteristicImpl(char_uuid, service_uuid, std::move(bt))) {}

    Characteristic::~Characteristic() {
        delete cImpl;
    }

    std::string Characteristic::uuid() {
        return cImpl->uuid();
    }

    uint8_t *Characteristic::read() {
        return cImpl->read();
    }

    void Characteristic::write_without_response(uint8_t *data, int length) {
        cImpl->write_without_response(data, length);
    }

    void Characteristic::write_with_response(uint8_t *data, int length) {
        cImpl->write_with_response(data, length);
    }

    void Characteristic::notify(const std::function<void(uint8_t *)> &callback) {
        cImpl->notify(callback);
    }

}
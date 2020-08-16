#include "characteristic.h"
#include "wrapper.h"
#include <string>
#include <utility>
#include <iostream>

namespace bluetooth {

    struct Characteristic::CharacteristicImpl {
    public:
        CharacteristicImpl(std::string char_uuid,
                           std::string service_uuid,
                           std::shared_ptr<wrapper::Wrapper> bt) :
                char_uuid(std::move(char_uuid)), service_uuid(std::move(service_uuid)), bt(std::move(bt)) {}

        ~CharacteristicImpl() {

        }

        std::string uuid() {
            return char_uuid;
        }

        std::vector<std::byte> read() {
            return bt->read(service_uuid, char_uuid);
        }

        void write_without_response(const std::vector<std::byte> &data) {
            bt->write_without_response(data, service_uuid, char_uuid);
        }

        void write_with_response(const std::vector<std::byte> &data) {
            bt->write_with_response(data, service_uuid, char_uuid);
        }

        void notify(const std::function<void(std::vector<std::byte>)> &callback) {
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

    std::vector<std::byte> Characteristic::read() {
        return cImpl->read();
    }

    void Characteristic::write_without_response(const std::vector<std::byte> &data) {
        cImpl->write_without_response(data);
    }

    void Characteristic::write_with_response(const std::vector<std::byte> &data) {
        cImpl->write_with_response(data);
    }

    void Characteristic::notify(const std::function<void(std::vector<std::byte>)> &callback) {
        cImpl->notify(callback);
    }

}
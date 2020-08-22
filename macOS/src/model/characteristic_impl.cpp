#include "characteristic.h"
#include "wrapper.h"
#include "conversions.h"
#include <string>
#include <utility>
#include <iostream>

namespace bluetooth {

    struct Characteristic::CharacteristicImpl {
    public:

        CharacteristicImpl() = default;

        CharacteristicImpl(std::string char_uuid,
                           std::string service_uuid,
                           std::shared_ptr<detail::wrapper::Wrapper> bt) :
                char_uuid(std::move(char_uuid)), service_uuid(std::move(service_uuid)), bt(std::move(bt)) {}

        ~CharacteristicImpl() = default;

        std::string uuid() {
            return char_uuid;
        }

        std::vector<std::byte> read() {
            return bt->read(service_uuid, char_uuid);
        }

        float read_float() {
            std::vector<std::byte> data = bt->read(service_uuid, char_uuid);
            return detail::conversion::bytes_to_float(data);
        }

        int read_int() {
            std::vector<std::byte> data = bt->read(service_uuid, char_uuid);
            return detail::conversion::bytes_to_int(data);
        }

        uint8_t read_uint8() {
            std::vector<std::byte> data = bt->read(service_uuid, char_uuid);
            return detail::conversion::bytes_to_uint8(data);
        }

        std::string read_string() {
            std::vector<std::byte> data = bt->read(service_uuid, char_uuid);
            return detail::conversion::bytes_to_string(data);
        }

        void write_without_response(const std::vector<std::byte> &data) {
            bt->write_without_response(data, service_uuid, char_uuid);
        }

        void write_without_response(float data) {
            bt->write_without_response(detail::conversion::float_to_bytes(data), service_uuid, char_uuid);
        }

        void write_without_response(int data) {
            bt->write_without_response(detail::conversion::int_to_bytes(data), service_uuid, char_uuid);
        }

        void write_without_response(uint8_t data) {
            bt->write_without_response(std::vector<std::byte>{(std::byte) data}, service_uuid, char_uuid);
        }

        void write_without_response(const std::string &data) {
            bt->write_without_response(detail::conversion::string_to_bytes(data), service_uuid, char_uuid);
        }

        void write_with_response(const std::vector<std::byte> &data) {
            bt->write_with_response(data, service_uuid, char_uuid);
        }

        void write_with_response(float data) {
            bt->write_with_response(detail::conversion::float_to_bytes(data), service_uuid, char_uuid);
        }

        void write_with_response(int data) {
            bt->write_with_response(detail::conversion::int_to_bytes(data), service_uuid, char_uuid);
        }

        void write_with_response(uint8_t data) {
            bt->write_with_response(std::vector<std::byte>{(std::byte) data}, service_uuid, char_uuid);
        }

        void write_with_response(const std::string &data) {
            bt->write_with_response(detail::conversion::string_to_bytes(data), service_uuid, char_uuid);
        }

        void notify(const std::function<void(std::vector<std::byte>)> &callback) {
            bt->notify(service_uuid, char_uuid, callback);
        }

    private:
        std::string char_uuid;
        std::string service_uuid;
        std::shared_ptr<detail::wrapper::Wrapper> bt;

    };

    Characteristic::Characteristic() : cImpl(new CharacteristicImpl()) {}

    Characteristic::Characteristic(const std::string &char_uuid,
                                   const std::string &service_uuid,
                                   std::shared_ptr<detail::wrapper::Wrapper> bt) :
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

    float Characteristic::read_float() {
        return cImpl->read_float();
    }

    int Characteristic::read_int() {
        return cImpl->read_int();
    }

    uint8_t Characteristic::read_uint8() {
        return cImpl->read_uint8();
    }

    std::string Characteristic::read_string() {
        return cImpl->read_string();
    }

    void Characteristic::write_without_response(const std::vector<std::byte> &data) {
        cImpl->write_without_response(data);
    }

    void Characteristic::write_without_response(float data) {
        cImpl->write_without_response(data);
    }

    void Characteristic::write_without_response(int data) {
        cImpl->write_without_response(data);
    }

    void Characteristic::write_without_response(uint8_t data) {
        cImpl->write_without_response(data);
    }

    void Characteristic::write_without_response(const std::string &data) {
        cImpl->write_without_response(data);
    }

    void Characteristic::write_with_response(const std::vector<std::byte> &data) {
        cImpl->write_with_response(data);
    }

    void Characteristic::write_with_response(float data) {
        cImpl->write_with_response(data);
    }

    void Characteristic::write_with_response(int data) {
        cImpl->write_with_response(data);
    }

    void Characteristic::write_with_response(uint8_t data) {
        cImpl->write_with_response(data);
    }

    void Characteristic::write_with_response(const std::string &data) {
        cImpl->write_with_response(data);
    }

    void Characteristic::notify(const std::function<void(std::vector<std::byte>)> &callback) {
        cImpl->notify(callback);
    }

}
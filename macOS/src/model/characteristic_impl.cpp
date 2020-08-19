#include "characteristic.h"
#include "wrapper.h"
#include <string>
#include <utility>
#include <iostream>

namespace bluetooth {

    struct Characteristic::CharacteristicImpl {
    public:

        CharacteristicImpl() = default;

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

        int read_int() {
            std::vector<std::byte> data = bt->read(service_uuid, char_uuid);
            if (data.size() > 4) {
                std::clog
                        << "warning: read data is greater than four bytes, will only use first four bytes to convert to int."
                        << std::endl;
            }
            return bytes_to_int(data);
        }

        uint8_t read_uint8() {
            std::vector<std::byte> data = bt->read(service_uuid, char_uuid);
            if (data.size() > 1) {
                std::clog << "warning: read data is greater than one byte, will only use first byte as return."
                          << std::endl;
            }
            return (uint8_t) data[0];
        }

        std::string read_string() {
            return bytes_to_string(bt->read(service_uuid, char_uuid));
        }

        void write_without_response(const std::vector<std::byte> &data) {
            bt->write_without_response(data, service_uuid, char_uuid);
        }

        void write_without_response(int data) {
            bt->write_without_response(int_to_bytes(data), service_uuid, char_uuid);
        }

        void write_without_response(uint8_t data) {
            bt->write_without_response(std::vector<std::byte>{(std::byte) data}, service_uuid, char_uuid);
        }

        void write_without_response(const std::string &data) {
            bt->write_without_response(string_to_bytes(data), service_uuid, char_uuid);
        }

        void write_with_response(const std::vector<std::byte> &data) {
            bt->write_with_response(data, service_uuid, char_uuid);
        }

        void write_with_response(int data) {
            bt->write_with_response(int_to_bytes(data), service_uuid, char_uuid);
        }

        void write_with_response(uint8_t data) {
            bt->write_with_response(std::vector<std::byte>{(std::byte) data}, service_uuid, char_uuid);
        }

        void write_with_response(const std::string &data) {
            bt->write_with_response(string_to_bytes(data), service_uuid, char_uuid);
        }

        void notify(const std::function<void(std::vector<std::byte>)> &callback) {
            bt->notify(service_uuid, char_uuid, callback);
        }

    private:
        std::string char_uuid;
        std::string service_uuid;
        std::shared_ptr<wrapper::Wrapper> bt;

        /**
         * Convert integer to bytes in little endian order.
         * @param data integer to convert.
         * @return vector of bytes.
         */
        std::vector<std::byte> int_to_bytes(int data) {
            std::vector<std::byte> byte_vector(4);
            for (int i = 0; i < 4; i++)
                byte_vector[3 - i] = std::byte((data >> (i * 8)));
            return byte_vector;
        }

        /**
         * Convert string to byte in little endian order.
         * @param data string to convert.
         * @return vector of bytes.
         */
        std::vector<std::byte> string_to_bytes(const std::string &data) {
            std::vector<std::byte> byte_vector(data.size());
            std::transform(data.begin(), data.end(), byte_vector.begin(),
                           [](char c) { return std::byte(c); });
            return byte_vector;
        }

        int bytes_to_int(const std::vector<std::byte> &bytes) {
            return int((unsigned char) (bytes[0]) << 24 |
                       (unsigned char) (bytes[1]) << 16 |
                       (unsigned char) (bytes[2]) << 8 |
                       (unsigned char) (bytes[3]));
        }

        std::string bytes_to_string(const std::vector<std::byte> &bytes) {
            return std::string(reinterpret_cast<char const *>(&bytes[0]), bytes.size());
        }
    };

    Characteristic::Characteristic() : cImpl(new CharacteristicImpl()) {}

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
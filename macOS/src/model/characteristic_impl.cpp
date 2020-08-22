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
            if (data.empty()) {
                std::clog
                        << "warning: read returned data of 0 bytes."
                        << std::endl;
                return 0;
            } else if (data.size() > 4) {
                std::clog
                        << "warning: read data is greater than four bytes, returning 0"
                        << std::endl;
                return 0;
            }
                // do error handling if size < 4
            else if (data.size() < 4) {
                std::clog
                        << "warning: read data is less than four bytes, returning 0"
                        << std::endl;
                return 0;
            }
            return bytes_to_float(data);
        }

        int read_int() {
            std::vector<std::byte> data = bt->read(service_uuid, char_uuid);
            if (data.empty()) {
                std::clog
                        << "warning: read returned data of 0 bytes."
                        << std::endl;
                return 0;
            } else if (data.size() > 4) {
                std::clog
                        << "warning: read data is greater than four bytes, will only use first four bytes to convert to int."
                        << std::endl;
            }
                // do error handling if size < 4
            else if (data.size() < 4) {
                std::clog
                        << "warning: read data is less than four bytes, will only use first byte to convert to int."
                        << std::endl;
                return (int) data[0];
            }
            return bytes_to_int(data);
        }

        uint8_t read_uint8() {
            std::vector<std::byte> data = bt->read(service_uuid, char_uuid);
            if (data.empty()) {
                std::clog
                        << "warning: read returned data of 0 bytes."
                        << std::endl;
                return 0;
            }
            if (data.size() > 1) {
                std::clog << "warning: read data is greater than one byte, will only use the last byte as return."
                          << std::endl;
            }
            return (uint8_t) data[data.size() - 1];
        }

        std::string read_string() {
            std::vector<std::byte> data = bt->read(service_uuid, char_uuid);
            if (data.empty()) {
                std::clog
                        << "warning: read returned data of 0 bytes."
                        << std::endl;
                return "";
            }
            return bytes_to_string(data);
        }

        void write_without_response(const std::vector<std::byte> &data) {
            bt->write_without_response(data, service_uuid, char_uuid);
        }

        void write_without_response(float data) {
            bt->write_without_response(float_to_bytes(data), service_uuid, char_uuid);
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

        void write_with_response(float data) {
            bt->write_with_response(float_to_bytes(data), service_uuid, char_uuid);
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
        std::shared_ptr<detail::wrapper::Wrapper> bt;

        std::vector<std::byte> float_to_bytes(float data) {
            std::vector<std::byte> bytes(sizeof(data));
            std::memcpy(bytes.data(), &data, sizeof(data));
            return bytes;
        }

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
            std::cout << std::endl;
            return byte_vector;
        }

        int bytes_to_int(const std::vector<std::byte> &bytes) {
            return int((unsigned char) (bytes[0]) << 24 |
                       (unsigned char) (bytes[1]) << 16 |
                       (unsigned char) (bytes[2]) << 8 |
                       (unsigned char) (bytes[3]));
        }

        float bytes_to_float(const std::vector<std::byte> &bytes) {
            float f;
            memcpy(&f, bytes.data(), sizeof(f));
            return f;
        }

        std::string bytes_to_string(const std::vector<std::byte> &bytes) {
            std::cout << std::endl;
            return std::string(reinterpret_cast<char const *>(&bytes[0]), bytes.size());
        }
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
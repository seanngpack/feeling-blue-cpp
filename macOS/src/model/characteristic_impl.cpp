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

        template<typename T>
        T read() {
            if constexpr(std::is_same_v<T, int>) {
                return detail::conversion::bytes_to_int(bt->read(service_uuid, char_uuid));
            } else if constexpr(std::is_same_v<T, uint8_t>) {
                return detail::conversion::bytes_to_uint8(bt->read(service_uuid, char_uuid));
            } else if constexpr(std::is_same_v<T, float>) {
                return detail::conversion::bytes_to_float(bt->read(service_uuid, char_uuid));
            } else if constexpr(std::is_same_v<T, std::string>) {
                return detail::conversion::bytes_to_string(bt->read(service_uuid, char_uuid));
            } else if constexpr(std::is_same_v<T, std::byte>) {
                return bt->read(service_uuid, char_uuid);
            }

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

    template<typename T>
    T Characteristic::read() {
        return cImpl->read<T>();
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

    ///@cond INTERNAL
    // explicit template instantiation, also above statement tells doxygen to ignore these
    template int Characteristic::read<int>();
    template uint8_t Characteristic::read<uint8_t>();
    template float Characteristic::read<float>();
    template std::string Characteristic::read<std::string>();
    ///@endcond
}
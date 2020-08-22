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
        struct Getter {
            Getter(Characteristic::CharacteristicImpl *impl) : impl(impl) {}

            T get() {
                std::cout << "NON VECTOR CALLED" << std::endl;
                if constexpr(std::is_same_v<T, int>) {
                    return detail::conversion::bytes_to_int(impl->bt->read(impl->service_uuid, impl->char_uuid));
                } else if constexpr(std::is_same_v<T, uint8_t>) {
                    return detail::conversion::bytes_to_uint8(impl->bt->read(impl->service_uuid, impl->char_uuid));
                } else if constexpr(std::is_same_v<T, float>) {
                    return detail::conversion::bytes_to_float(impl->bt->read(impl->service_uuid, impl->char_uuid));
                } else if constexpr(std::is_same_v<T, std::string>) {
                    return detail::conversion::bytes_to_string(impl->bt->read(impl->service_uuid, impl->char_uuid));
                }
            }

            Characteristic::CharacteristicImpl *impl;
        };

        template<typename T>
        struct Getter<std::vector<T> > {
            Getter(Characteristic::CharacteristicImpl *impl) : impl(impl) {}

            std::vector<T> get() {
                std::cout << "VECTOR CALLED" << std::endl;
                if constexpr(std::is_same_v<T, std::byte>) {
                    return impl->bt->read(impl->service_uuid, impl->char_uuid);
                }
            }

            Characteristic::CharacteristicImpl *impl;
        };

        template<typename T>
        T read() {
            return Getter<T>(this).get();
        }


        template<typename T>
        void write_without_response(T data) {
            if constexpr(std::is_same_v<T, int>) {
                bt->write_without_response(detail::conversion::int_to_bytes(data), service_uuid, char_uuid);
            } else if constexpr(std::is_same_v<T, uint8_t>) {
                bt->write_without_response(detail::conversion::uint8_to_bytes(data), service_uuid, char_uuid);
            } else if constexpr(std::is_same_v<T, float>) {
                bt->write_without_response(detail::conversion::float_to_bytes(data), service_uuid, char_uuid);
            } else if constexpr(std::is_same_v<T, std::string>) {
                bt->write_without_response(detail::conversion::string_to_bytes(data), service_uuid, char_uuid);
            }
        }

        template<typename T>
        void write_without_response(const std::vector<T> &data) {
            if constexpr(std::is_same_v<T, std::byte>) {
                bt->write_without_response(data, service_uuid, char_uuid);
            }
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

    template<typename T>
    void Characteristic::write_without_response(T data) {
        cImpl->write_without_response<T>(data);
    }

    template<typename T>
    void Characteristic::write_without_response(const std::vector<T> &data) {
        cImpl->write_without_response<T>(data);
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
    // read methods
    template int Characteristic::read<int>();

    template uint8_t Characteristic::read<uint8_t>();

    template float Characteristic::read<float>();

    template std::string Characteristic::read<std::string>();

    template std::vector<std::byte> Characteristic::read<std::vector<std::byte>>();

    //write_without_response methods
    template void Characteristic::write_without_response<uint8_t>(uint8_t data);

    template void Characteristic::write_without_response<int>(int data);

    template void Characteristic::write_without_response<float>(float data);

    template void Characteristic::write_without_response<std::string>(std::string data);

    template void Characteristic::write_without_response<std::byte>(const std::vector<std::byte> &data);

//    template
//    class Getter<uint8_t>;
//
//    template
//    class Getter<int>;
//
//    template
//    class Getter<float>;
//
//    template
//    class Getter<std::string>;


    ///@endcond
}
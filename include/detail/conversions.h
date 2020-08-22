/**
 * This header file contains conversions from primitive types to bytes and vice-versa.
 */

#ifndef FEELING_BLUE_CONVERSIONS_H
#define FEELING_BLUE_CONVERSIONS_H

#include <vector>
#include <string>

namespace bluetooth {
    namespace detail {
        namespace conversion {
            /*-------------------------------------------------------
                                    To Bytes

            ---------------------------------------------------------*/

            /**
             * Convert the float to bytes. Assumes little endian order.
             * @param data float to convert.
             * @return vector of bytes.
             */
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
             * Convert a non-terminated std::string to byte in little endian order.
             * @param data string to convert.
             * @return vector of bytes.
             */
            std::vector<std::byte> string_to_bytes(const std::string &data) {
                std::vector<std::byte> byte_vector(data.size());
                std::transform(data.begin(), data.end(), byte_vector.begin(),
                               [](char c) { return std::byte(c); });
                return byte_vector;
            }



            /*-------------------------------------------------------
                                    From Bytes

            ---------------------------------------------------------*/

            /**
             * Convert vector of bytes to signed 4-byte integer. Assumes little endian order.
             * @param bytes vector of bytes to convert.
             * @return integer.
             */
            int bytes_to_int(const std::vector<std::byte> &bytes) {
                return int((unsigned char) (bytes[0]) << 24 |
                           (unsigned char) (bytes[1]) << 16 |
                           (unsigned char) (bytes[2]) << 8 |
                           (unsigned char) (bytes[3]));
            }

            /**
             * Convert vector of bytes to signed 4-byte float. Assumes little endian order.
             * @param bytes vector of bytes to convert.
             * @return float.
             */
            float bytes_to_float(const std::vector<std::byte> &bytes) {
                float f;
                memcpy(&f, bytes.data(), sizeof(f));
                return f;
            }

            /**
             * Convert vector of bytes representing a string to std::string.
             * @param bytes vector of bytes to convert.
             * @return string.
             */
            std::string bytes_to_string(const std::vector<std::byte> &bytes) {
                return std::string(reinterpret_cast<char const *>(&bytes[0]), bytes.size());
            }

        }
    }
}


#endif //FEELING_BLUE_CONVERSIONS_H

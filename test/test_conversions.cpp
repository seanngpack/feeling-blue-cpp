#include "conversions.h"
#include "gtest/gtest.h"
#include <vector>
#include <iostream>

using namespace bluetooth::detail::conversion;
using bytes = std::vector<std::byte>;
using byte = std::byte;

TEST(ConversionTests, TestUint8) {
    uint8_t u0 = 0;
    uint8_t u1 = 100;

    bytes i0_bytes = uint8_to_bytes(u0);
    bytes i1_bytes = uint8_to_bytes(u1);

    ASSERT_EQ(bytes_to_uint8(i0_bytes), u0);
    ASSERT_EQ(bytes_to_uint8(i1_bytes), u1);
}

// test illegal byte conversion cases
TEST(ConversionTests, TestUint8ZeroReturn) {
    bytes b0 = {};
    bytes b1 = {(byte) 100, (byte) 110};
    ASSERT_EQ(bytes_to_uint8(b0), 0);
    ASSERT_EQ(bytes_to_uint8(b1), 0);
}

// Test int conversions.
TEST(ConversionTests, TestInt) {
    int i0 = 0;
    int i1 = -100;
    int i2 = 100239848;

    bytes i0_bytes = int_to_bytes(i0);
    bytes i1_bytes = int_to_bytes(i1);
    bytes i2_bytes = int_to_bytes(i2);

    ASSERT_EQ(bytes_to_int(i0_bytes), i0);
    ASSERT_EQ(bytes_to_int(i1_bytes), i1);
    ASSERT_EQ(bytes_to_int(i2_bytes), i2);
}

// test illegal byte conversion cases
TEST(ConversionTests, TestIntZeroReturn) {
    bytes b0 = {};
    bytes b1 = {(byte) 0, (byte) 1};
    bytes b2 = {(byte) 0};
    bytes b3 = {(byte) 1, (byte) 2, (byte) 3, (byte) 0, (byte) 6};

    ASSERT_EQ(bytes_to_int(b0), 0);
    ASSERT_EQ(bytes_to_int(b1), 0);
    ASSERT_EQ(bytes_to_int(b2), 0);
    ASSERT_EQ(bytes_to_int(b3), 0);
}

TEST(ConversionTests, TestFloat) {
    float f0 = 0.0;
    float f1 = 1239.1234;
    float f2 = -.00234;

    bytes f0_bytes = float_to_bytes(f0);
    bytes f1_bytes = float_to_bytes(f1);
    bytes f2_bytes = float_to_bytes(f2);

    ASSERT_FLOAT_EQ(bytes_to_float(f0_bytes), f0);
    ASSERT_FLOAT_EQ(bytes_to_float(f1_bytes), f1);
    ASSERT_FLOAT_EQ(bytes_to_float(f2_bytes), f2);
}

TEST(ConversionTests, TestFloatZeroReturn) {
    bytes b0 = {};
    bytes b1 = {(byte) 0, (byte) 1};
    bytes b2 = {(byte) 1, (byte) 2, (byte) 3, (byte) 0, (byte) 6};

    ASSERT_FLOAT_EQ(bytes_to_float(b0), 0);
    ASSERT_FLOAT_EQ(bytes_to_float(b1), 0);
    ASSERT_FLOAT_EQ(bytes_to_float(b2), 0);
}

TEST(ConversionTests, TestDouble) {
    double d0 = 0.0;
    double d1 = 1239.1234;
    double d2 = -.00234;

    bytes d0_bytes = double_to_bytes(d0);
    bytes d1_bytes = double_to_bytes(d1);
    bytes d2_bytes = double_to_bytes(d2);

    ASSERT_FLOAT_EQ(bytes_to_double(d0_bytes), d0);
    ASSERT_FLOAT_EQ(bytes_to_double(d1_bytes), d1);
    ASSERT_FLOAT_EQ(bytes_to_double(d2_bytes), d2);
}

TEST(ConversionTests, TestDoubleZeroReturn) {
    bytes b0 = {};
    bytes b1 = {(byte) 0, (byte) 1};
    bytes b2 = {(byte) 1, (byte) 2, (byte) 3, (byte) 0, (byte) 6};

    ASSERT_FLOAT_EQ(bytes_to_double(b0), 0);
    ASSERT_FLOAT_EQ(bytes_to_double(b1), 0);
    ASSERT_FLOAT_EQ(bytes_to_double(b2), 0);
}

TEST(ConversionTests, TestString) {
    std::string s1 = "hello this is cool";
    std::string s2 = "hel!$_)#lo this sad-3%2is 238cooldfskdf'si83";
    std::string s3 = "\0hel!$_)#lo this sad-3%2is 238cooldfskdf'si83\0";

    bytes s1_bytes = string_to_bytes(s1);
    bytes s2_bytes = string_to_bytes(s2);
    bytes s3_bytes = string_to_bytes(s3);

    ASSERT_TRUE(bytes_to_string(s1_bytes) == s1);
    ASSERT_TRUE(bytes_to_string(s2_bytes) == s2);
    ASSERT_TRUE(bytes_to_string(s3_bytes) == s3);
}

TEST(ConversionTests, TestStringZeroReturn) {
    bytes b0 = {};
    ASSERT_TRUE(bytes_to_string(b0).empty());
}


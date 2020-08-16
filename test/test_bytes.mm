#include <gtest/gtest.h>
#include <iostream>
#include <cstddef>
#import <Foundation/Foundation.h>

TEST(TestBytes, testNSDataToVectorBytes) {
    const unsigned char bytes[] = {0x01, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    NSData *data = [NSData dataWithBytes:bytes length:sizeof(bytes)];

    NSLog(@"%@", data);

    auto length = (uint8_t) [data length];
    auto *uArr = new uint8_t[length];
    memcpy(uArr, [data bytes], length);

    std::vector<std::byte> swag;

    for (int i = 0; i < length; i++) {
        uint8_t x = uArr[i];
        swag.push_back((std::byte) x);
    }

    for (auto const &b: swag) {
        std::cout << (int) b << std::endl;
    }

    ASSERT_EQ(bytes[0], (int) swag[0]);
    ASSERT_EQ(bytes[9], (int) swag[9]);
}


// what does the length field of NSData refer to?
// okay length is the same as sizeof, it's the total size of the array...
// or as they put it "# of bytes"
TEST(TestBytes, testLengthNSData) {
    const uint16_t bytes[] = {1, 2, 3};
    int bytes_size = sizeof(bytes);
    NSData *bytesNS = [NSData dataWithBytes:bytes length:sizeof(bytes)];
    int bytesNS_size = (int) [bytesNS length];
    NSLog(@"%@", bytesNS);
    ASSERT_EQ(bytes_size, bytesNS_size);
}

// convert std::vector byte to NSDATA
TEST(TestBytes, testVectorByteToNSData) {
    std::vector<std::byte> bytes = {(std::byte) 1, (std::byte) 2, (std::byte) 3};
    NSData *bytesNS = [NSData dataWithBytes:bytes.data() length:bytes.size() * sizeof(std::byte)];
    NSLog(@"%@", bytesNS);
}
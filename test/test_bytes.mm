#include <gtest/gtest.h>
#include <iostream>
#import <Foundation/Foundation.h>

TEST(TestBytes, testNSDataToVectorBytes) {
    const unsigned char bytes[] = {1,2,3,4,5,6,7,8,9,10};
    std::cout << sizeof bytes << std::endl;
    NSData *data = [NSData dataWithBytes:bytes length:sizeof(bytes)];
    NSLog(@"%@", data);
    ASSERT_TRUE(true);
}
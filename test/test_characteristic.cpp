#include <gtest/gtest.h>
#include <iostream>
#include <cstddef>
#import <Foundation/Foundation.h>
#include <characteristic.h>

class CharacteristicTest : public ::testing::Test
{
protected:
     bluetooth::Characteristic *characteristic;
    virtual void SetUp()
    {
        characteristic = new bluetooth::Characteristic();
    }

    virtual void TearDown()
    {
        delete characteristic;
    }
};


TEST(CharacteristicTest, testNSDataToVectorBytes) {
}
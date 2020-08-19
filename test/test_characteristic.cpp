#include <gtest/gtest.h>
#include <iostream>
#include <cstddef>
#include "characteristic.h"

#define private public
#include <characteristic.h>

class CharacteristicTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        characteristic = new bluetooth::Characteristic();
    }

    virtual void TearDown() {
        delete characteristic;
    }

    bluetooth::Characteristic *characteristic;
};


TEST_F(CharacteristicTest, testIntConversions) {

}

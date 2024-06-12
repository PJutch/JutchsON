#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Bool, writeBoolTrue) {
    EXPECT_EQ(JutchsON::writeBool(true), "true");
}

TEST(Bool, writeBoolFalse) {
    EXPECT_EQ(JutchsON::writeBool(false), "false");
}

TEST(Bool, writeBoolEmpty) {
    EXPECT_EQ(JutchsON::writeBool(true, true), "");
}

TEST(Bool, writeBoolFalseAllowEmpty) {
    EXPECT_EQ(JutchsON::writeBool(false, true), "false");
}

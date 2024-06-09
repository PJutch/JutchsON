#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Strip, strip) {
    EXPECT_EQ(JutchsON::strip(" \n abcde\t    "), "abcde");
}

TEST(Strip, stripBegin) {
    EXPECT_EQ(JutchsON::strip(" \n abcde"), "abcde");
}

TEST(Strip, stripEnd) {
    EXPECT_EQ(JutchsON::strip("abcde\t    "), "abcde");
}

TEST(Strip, stripSame) {
    EXPECT_EQ(JutchsON::strip("abcde"), "abcde");
}

TEST(Strip, stripEmpty) {
    EXPECT_EQ(JutchsON::strip(""), "");
}

TEST(Strip, stripSpaceOnly) {
    EXPECT_EQ(JutchsON::strip("  \t    "), "");
}


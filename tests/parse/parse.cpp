#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Parse, parseUnsignedInt) {
	EXPECT_EQ(JutchsON::parse<unsigned int>("123"), 123u);
}

TEST(Parse, parseInt) {
	EXPECT_EQ(JutchsON::parse<int>("-123"), -123);
}

TEST(Parse, parseFloat) {
	EXPECT_DOUBLE_EQ(*JutchsON::parse<double>("1.23"), 1.23);
}

TEST(Parse, parseString) {
	EXPECT_EQ(JutchsON::parse<std::string>("abc de"), "abc de");
}

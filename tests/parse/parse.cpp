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

TEST(Parse, parseBool) {
	EXPECT_TRUE(*JutchsON::parse<bool>("true"));
}

TEST(Parse, parseBoolEmpty) {
	EXPECT_EQ(JutchsON::parse<bool>("", JutchsON::Context::LINE), 
		JutchsON::ParseResult<bool>::makeError({0, 0}, "Empty bool isn't allowed"));
}

TEST(Parse, parseBoolEmptyAllowed) {
	EXPECT_TRUE(*JutchsON::parse<bool>("", JutchsON::Context::LINE_REST));
}

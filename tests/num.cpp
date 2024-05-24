#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Num, parseUint) {
	EXPECT_EQ(JutchsON::parseUint("123"), JutchsON::ParseResult{123u});
}

TEST(Num, parseUintInt) {
	EXPECT_EQ(JutchsON::parseUint<int>("123"), JutchsON::ParseResult{123});
}

TEST(Num, parseUintZero) {
	EXPECT_EQ(JutchsON::parseUint("0"), JutchsON::ParseResult{0u});
}

TEST(Num, parseUintManyZero) {
	EXPECT_EQ(JutchsON::parseUint("000"), JutchsON::ParseResult{0u});
}

TEST(Num, parseUintGarbage) {
	EXPECT_EQ(JutchsON::parseUint("123garbage"), JutchsON::ParseResult<unsigned int>::makeError({0, 3}, "Number can't contain char g"));
}

TEST(Num, parseUintSep) {
    EXPECT_EQ(JutchsON::parseUint("12'3"), JutchsON::ParseResult{123u});
}

TEST(Num, parseUintSeps) {
	EXPECT_EQ(JutchsON::parseUint("1'2'3"), JutchsON::ParseResult{123u});
}

TEST(Num, parseUintStartSep) {
	EXPECT_EQ(JutchsON::parseUint("'123"), JutchsON::ParseResult<unsigned int>::makeError({0, 0}, "Number can't start with '"));
}

TEST(Num, parseUintEndSep) {
	EXPECT_EQ(JutchsON::parseUint("123'"), JutchsON::ParseResult<unsigned int>::makeError({0, 3}, "Number can't end with '"));
}

TEST(Num, parseUintDoubleSep) {
	EXPECT_EQ(JutchsON::parseUint("12''3"), JutchsON::ParseResult<unsigned int>::makeError({0, 3}, "Number can't contain double '"));
}

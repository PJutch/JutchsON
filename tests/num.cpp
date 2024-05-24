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
	EXPECT_EQ(JutchsON::parseUint("123garbage"), JutchsON::ParseResult<unsigned int>::makeError({0, 3}, "Number can't contain char 'g'"));
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

TEST(Num, parseInt) {
	EXPECT_EQ(JutchsON::parseInt("123"), 123);
}

TEST(Num, parseIntError) {
	EXPECT_EQ(JutchsON::parseInt("123garbage"), JutchsON::ParseResult<int>::makeError({0, 3}, "Number can't contain char 'g'"));
}

TEST(Num, parseIntLongLong) {
	EXPECT_EQ(JutchsON::parseInt<long long>("123"), 123ll);
}

TEST(Num, parseIntPlus) {
	EXPECT_EQ(JutchsON::parseInt("+123"), 123);
}

TEST(Num, parseIntPlusError) {
	EXPECT_EQ(JutchsON::parseInt("+123garbage"), JutchsON::ParseResult<int>::makeError({0, 4}, "Number can't contain char 'g'"));
}

TEST(Num, parseIntMinus) {
	EXPECT_EQ(JutchsON::parseInt("-123"), -123);
}

TEST(Num, parseIntMinusError) {
	EXPECT_EQ(JutchsON::parseInt("-123garbage"), JutchsON::ParseResult<int>::makeError({0, 4}, "Number can't contain char 'g'"));
}

TEST(Num, powIntExp) {
	EXPECT_EQ(JutchsON::powIntExp(10, 5), 100'000);
}

TEST(Num, powIntExpFloat) {
	EXPECT_EQ(JutchsON::powIntExp(10.0, 10), 10'000'000'000.0);
}

TEST(Num, powIntExp1) {
	EXPECT_EQ(JutchsON::powIntExp(10, 1), 10);
}

TEST(Num, powIntExp0) {
	EXPECT_EQ(JutchsON::powIntExp(10, 0), 1);
}

TEST(Num, powIntExpM1) {
	EXPECT_EQ(JutchsON::powIntExp(10.0, -1), 0.1);
}

TEST(Num, powIntExpNegative) {
	EXPECT_FLOAT_EQ(JutchsON::powIntExp(10.0, -5), 1e-5);
}

TEST(Num, parseNonnegativeFloat) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("1.23"), 1.23);
}

TEST(Num, parseNonnegativeFloatInt) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("123"), 123.0);
}

TEST(Num, parseNonnegativeFloatDot0) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("123.0"), 123.0);
}

TEST(Num, parseNonnegativeFloatDotLeading0) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("1.01"), 1.01);
}

TEST(Num, parseNonnegativeFloatLess1) {
	EXPECT_FLOAT_EQ(JutchsON::parseNonnegativeFloat("0.01").getOk(), 0.01);
}

TEST(Num, parseNonnegativeFloat0) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("0"), 0.0);
}

TEST(Num, parseNonnegativeFloat0Dot0) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("0.0"), 0.0);
}

TEST(Num, parseNonnegativeFloatGarbage) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("123garbage"), JutchsON::ParseResult<double>::makeError({0, 3}, "Number can't contain char 'g'"));
}

TEST(Num, parseNonnegativeFloatDotGarbage) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("1.0garbage"), JutchsON::ParseResult<double>::makeError({0, 3}, "Number can't contain char 'g'"));
}

TEST(Num, parseFloat) {
	EXPECT_EQ(JutchsON::parseFloat("1.23"), 1.23);
}

TEST(Num, parseFloatPlus) {
	EXPECT_EQ(JutchsON::parseFloat("+1.23"), 1.23);
}

TEST(Num, parseFloatMinus) {
	EXPECT_EQ(JutchsON::parseFloat("-1.23"), -1.23);
}

TEST(Num, parseFloatMinus0) {
	EXPECT_EQ(JutchsON::parseFloat("-0"), -0.0);
}

TEST(Num, parseFloatMinusGreaterMinus1) {
	EXPECT_FLOAT_EQ(JutchsON::parseFloat("-0.01").getOk(), -0.01);
}

TEST(Num, parseNonnegativeFloatSignGarbage) {
	EXPECT_EQ(JutchsON::parseFloat("+123garbage"), JutchsON::ParseResult<double>::makeError({0, 4}, "Number can't contain char 'g'"));
}

TEST(Num, parseNonnegativeFloatSignDotGarbage) {
	EXPECT_EQ(JutchsON::parseFloat("+1.0garbage"), JutchsON::ParseResult<double>::makeError({0, 4}, "Number can't contain char 'g'"));
}

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

TEST(Num, parseUintExp) {
	EXPECT_EQ(JutchsON::parseUint("12e5"), 1'200'000u);
}

TEST(Num, parseUint1Exp) {
	EXPECT_EQ(JutchsON::parseUint("1e5"), 100'000u);
}

TEST(Num, parseUintNoExp) {
	EXPECT_EQ(JutchsON::parseUint("12e"), JutchsON::ParseResult<unsigned int>::makeError({0, 3}, "Exponent must follow e"));
}

TEST(Num, parseUintGarbageExp) {
	EXPECT_EQ(JutchsON::parseUint("12egarbage"), JutchsON::ParseResult<unsigned int>::makeError({0, 3}, "Number can't contain char 'g'"));
}

TEST(Num, parseUintNegExp) {
	EXPECT_EQ(JutchsON::parseUint("12e-1"), JutchsON::ParseResult<unsigned int>::makeError({0, 3}, "Integer exponent must be > 0"));
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
	EXPECT_DOUBLE_EQ(JutchsON::powIntExp(10.0, -5), 1e-5);
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
	EXPECT_DOUBLE_EQ(*JutchsON::parseNonnegativeFloat("0.01"), 0.01);
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

TEST(Num, parseNonnegativeFloatExp) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("1.23e3"), 1230.0);
}

TEST(Num, parseNonnegativeFloatNegExp) {
	EXPECT_DOUBLE_EQ(*JutchsON::parseNonnegativeFloat("1.23e-2"), 1.23e-2);
}

TEST(Num, parseNonnegativeFloatNoExp) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("1.23e"), JutchsON::ParseResult<double>::makeError({0, 5}, "Exponent must follow e"));
}

TEST(Num, parseNonnegativeFloatIntExp) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("123e2"), 12'300.0);
}

TEST(Num, parseNonnegativeFloatIntNegExp) {
	EXPECT_DOUBLE_EQ(*JutchsON::parseNonnegativeFloat("123e-2"), 1.23);
}

TEST(Num, parseNonnegativeFloatIntNoExp) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("123e"), JutchsON::ParseResult<double>::makeError({0, 4}, "Exponent must follow e"));
}

TEST(Num, parseNonnegativeFloatIntDotExp) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("123.e2"), 12'300.0);
}

TEST(Num, parseNonnegativeFloatIntDotNegExp) {
	EXPECT_DOUBLE_EQ(*JutchsON::parseNonnegativeFloat("123.e-2"), 1.23);
}

TEST(Num, parseNonnegativeFloatIntDotNoExp) {
	EXPECT_EQ(JutchsON::parseNonnegativeFloat("123.e"), JutchsON::ParseResult<double>::makeError({0, 5}, "Exponent must follow e"));
}

TEST(Num, parseNonnegativeFloatNaN) {
	EXPECT_TRUE(isnan(*JutchsON::parseNonnegativeFloat("nan")));
}

TEST(Num, parseNonnegativeFloatInf) {
	double res = *JutchsON::parseNonnegativeFloat("inf");
	EXPECT_TRUE(isinf(res) && res > 0);
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
	EXPECT_DOUBLE_EQ(*JutchsON::parseFloat("-0.01"), -0.01);
}

TEST(Num, parseFloatSignGarbage) {
	EXPECT_EQ(JutchsON::parseFloat("+123garbage"), JutchsON::ParseResult<double>::makeError({0, 4}, "Number can't contain char 'g'"));
}

TEST(Num, parseFloatSignDotGarbage) {
	EXPECT_EQ(JutchsON::parseFloat("+1.0garbage"), JutchsON::ParseResult<double>::makeError({0, 4}, "Number can't contain char 'g'"));
}


TEST(Num, parseFloatExp) {
	EXPECT_EQ(JutchsON::parseFloat("1.23e5"), 123'000.0);
}

TEST(Num, parseFloatMinusExp) {
	EXPECT_EQ(JutchsON::parseFloat("-1.23e5"), -123'000.0);
}

TEST(Num, parseFloatSignInf) {
	double res = *JutchsON::parseFloat("+inf");
	EXPECT_TRUE(isinf(res) && res > 0);
}

TEST(Num, parseFloatNegInf) {
	double res = *JutchsON::parseFloat("-inf");
	EXPECT_TRUE(isinf(res) && res < 0);
}

TEST(Num, parseGenericUnsignedInt) {
	EXPECT_EQ(JutchsON::parse<unsigned int>("123"), 123u);
}

TEST(Num, parseGenericInt) {
	EXPECT_EQ(JutchsON::parse<int>("-123"), -123);
}

TEST(Num, parseGenericFloat) {
	EXPECT_DOUBLE_EQ(*JutchsON::parse<double>("1.23"), 1.23);
}

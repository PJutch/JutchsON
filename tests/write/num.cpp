#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Num, writeUint) {
	EXPECT_EQ(JutchsON::writeUint(123u), "123");
}

TEST(Num, writeUintInt) {
	EXPECT_EQ(JutchsON::writeUint(123), "123");
}

TEST(Num, writeUintZero) {
	EXPECT_EQ(JutchsON::writeUint(0u), "0");
}

TEST(Num, writeInt) {
	EXPECT_EQ(JutchsON::writeInt(123), "123");
}

TEST(Num, writeIntUnsgined) {
	EXPECT_EQ(JutchsON::writeInt(123u), "123");
}

TEST(Num, writeIntZero) {
	EXPECT_EQ(JutchsON::writeInt(0), "0");
}

TEST(Num, writeIntNegative) {
	EXPECT_EQ(JutchsON::writeInt(-123), "-123");
}

TEST(Num, writeNonnegativeFloat) {
	EXPECT_EQ(JutchsON::writeNonnegativeFloat(1.25), "1.25");
}

TEST(Num, writeNonnegativeFloatZeroInt) {
	EXPECT_EQ(JutchsON::writeNonnegativeFloat(0.25), "0.25");
}

TEST(Num, writeNonnegativeFloatLeadingZeroFloat) {
	EXPECT_EQ(JutchsON::writeNonnegativeFloat(0.0625), "0.0625");
}

TEST(Num, writeNonnegativeFloatInt) {
	EXPECT_EQ(JutchsON::writeNonnegativeFloat(123.), "123");
}

TEST(Num, writeNonnegativeFloatNan) {
	EXPECT_EQ(JutchsON::writeNonnegativeFloat(NAN), "nan");
}

TEST(Num, writeNonnegativeFloatInf) {
	EXPECT_EQ(JutchsON::writeNonnegativeFloat(INFINITY), "inf");
}

TEST(Num, writeFloat) {
	EXPECT_EQ(JutchsON::writeFloat(1.25), "1.25");
}

TEST(Num, writeFloatZeroInt) {
	EXPECT_EQ(JutchsON::writeFloat(0.25), "0.25");
}

TEST(Num, writeFloatNegative) {
	EXPECT_EQ(JutchsON::writeFloat(-1.25), "-1.25");
}

TEST(Num, writeFloatZero) {
	EXPECT_EQ(JutchsON::writeFloat(0.), "0");
}

TEST(Num, writeFloatNegativeZero) {
	EXPECT_EQ(JutchsON::writeFloat(-0.), "-0");
}

TEST(Num, writeFloatNegativeInf) {
	EXPECT_EQ(JutchsON::writeFloat(-INFINITY), "-inf");
}

TEST(Num, writeGenericUnsignedInt) {
	EXPECT_EQ(JutchsON::write(123u), "123");
}

TEST(Num, writeGenericInt) {
	EXPECT_EQ(JutchsON::write(-123), "-123");
}

TEST(Num, writeGenericFloat) {
	EXPECT_EQ(JutchsON::write(1.25), "1.25");
}

#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Num, parseUint) {
	EXPECT_EQ(JutchsON::parseUint("123"), JutchsON::ParseResult{123u});
}

TEST(Num, parseUintZero) {
	EXPECT_EQ(JutchsON::parseUint("0"), JutchsON::ParseResult{0u});
}

TEST(Num, parseUintManyZero) {
	EXPECT_EQ(JutchsON::parseUint("000"), JutchsON::ParseResult{0u});
}

// TEST(Num, parseUintSpaces) {
// 	   EXPECT_EQ(JutchsON::parseUint("  123 "), JutchsON::ParseResult{123u});
// }

// TEST(Num, parseUintSeps) {
//     EXPECT_EQ(JutchsON::parseUint("12'3"), JutchsON::ParseResult{123u});
// }
 
#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Escape, digitChar) {
    EXPECT_EQ(JutchsON::digitChar(2), '2');
}

TEST(Escape, digitChar0) {
    EXPECT_EQ(JutchsON::digitChar(0), '0');
}

TEST(Escape, digitChar9) {
    EXPECT_EQ(JutchsON::digitChar(9), '9');
}

TEST(Escape, digitCharLetter) {
    EXPECT_EQ(JutchsON::digitChar(11), 'B');
}

TEST(Escape, digitCharA) {
    EXPECT_EQ(JutchsON::digitChar(10), 'A');
}

TEST(Escape, digitCharZ) {
    EXPECT_EQ(JutchsON::digitChar(35), 'Z');
}

TEST(Escape, escapeHex) {
    EXPECT_EQ(JutchsON::escapeHex('\x14'), "'\\x14'");
}

TEST(Escape, escapeHexLeading0) {
    EXPECT_EQ(JutchsON::escapeHex('\x05'), "'\\x05'");
}

TEST(Escape, escapeHexLeadingF) {
    EXPECT_EQ(JutchsON::escapeHex('\xF5'), "'\\xF5'");
}

TEST(Escape, escapeChar) {
    EXPECT_EQ(JutchsON::escapeChar("a", 0), "'a'");
}

TEST(Escape, escapeCharEscapeSeq) {
    EXPECT_EQ(JutchsON::escapeChar("\n", 0), "'\\n'");
}

TEST(Escape, escapeCharNoEscapeSeq) {
    EXPECT_EQ(JutchsON::escapeChar("\x05", 0), "'\\x05'");
}

TEST(Escape, escapeCharUtf8) {
    EXPECT_EQ(JutchsON::escapeChar("\u044F", 0), "'\u044F'");
}

TEST(Escape, escapeCharUtf8NoHeader) {
    EXPECT_EQ(JutchsON::escapeChar("\x8F", 0), "'\\x8F'");
}

TEST(Escape, escapeCharUtf8Trimmed) {
    EXPECT_EQ(JutchsON::escapeChar("\xD1", 0), "'\\xD1'");
}

TEST(Escape, escapeCharNotUtf8) {
    EXPECT_EQ(JutchsON::escapeChar("\xFE", 0), "'\\xFE'");
}

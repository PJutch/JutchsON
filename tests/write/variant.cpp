#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Variant, writeVariant) {
    EXPECT_EQ(JutchsON::writeVariant("abc", "de"), "abc de");
}

TEST(Variant, writeVariantNoValue) {
    EXPECT_EQ(JutchsON::writeVariant("abc", ""), "abc");
}

TEST(Variant, wrtieVariantQuoted) {
    EXPECT_EQ(JutchsON::writeVariant("abc", "de", true), "<abc de>");
}

TEST(Variant, wrtieVariantQuotedNoValue) {
    EXPECT_EQ(JutchsON::writeVariant("abc", "", true), "<abc>");
}

TEST(Variant, wrtieVariantEmptyQuoted) {
    EXPECT_EQ(JutchsON::writeVariant("", "", true), "<>");
}

TEST(Variant, wrtieStdVariantFirst) {
    EXPECT_EQ((JutchsON::write(std::variant<int, std::string>{123})), "0 123");
}

TEST(Variant, writeStdVariantSecond) {
    EXPECT_EQ((JutchsON::write(std::variant<int, std::string>{"abc def j"})), "1 abc def j");
}

TEST(Variant, wrtieStdVariantChevrons) {
    EXPECT_EQ((JutchsON::write(std::variant<int, std::string>{123}, JutchsON::Context::OBJECT)), "<0 123>");
}

TEST(Variant, wrtieStdVariantValueContext) {
    EXPECT_EQ((JutchsON::write(std::variant<int, bool>{true})), "1");
}

TEST(Variant, wrtieStdVariantChevronsValueContext) {
    EXPECT_EQ((JutchsON::write(std::variant<int, bool>{true}, JutchsON::Context::OBJECT)), "<1>");
}

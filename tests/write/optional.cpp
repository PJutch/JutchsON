#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Optional, writeOptional) {
    EXPECT_EQ(JutchsON::writeOptional("abc de"), "abc de");
}

TEST(Optional, writeOptionalEmpty) {
    EXPECT_EQ(JutchsON::writeOptional(""), "");
}

TEST(Optional, writeOptionalQuoteNull) {
    EXPECT_EQ(JutchsON::writeOptional("null"), "<null>");
}

TEST(Optional, writeOptionalQuoteQuoted) {
    EXPECT_EQ(JutchsON::writeOptional("<abc de>"), "<<abc de>>");
}

TEST(Optional, writeOptionalQuoteQuotedNull) {
    EXPECT_EQ(JutchsON::writeOptional("<null>"), "<<null>>");
}

TEST(Optional, writeStdOpitonal) {
    EXPECT_EQ(JutchsON::write(std::optional{123}), "123");
}

TEST(Optional, writeStdOpitonalNull) {
    EXPECT_EQ(JutchsON::write(std::optional<int>{}), "null");
}

TEST(Optional, writeStdOpitonalNested) {
    EXPECT_EQ(JutchsON::write(std::optional{std::optional{123}}), "123");
}

TEST(Optional, writeStdOpitonalNestedNull) {
    EXPECT_EQ(JutchsON::write(std::optional<std::optional<int>>{}), "null");
}

TEST(Optional, writeStdOpitonalNestedAngleBracketsNull) {
    EXPECT_EQ(JutchsON::write(std::optional<std::optional<int>>{{}}), "<null>");
}

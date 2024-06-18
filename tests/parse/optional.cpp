#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Optional, parseOptional) {
    EXPECT_EQ(**JutchsON::parseOptional("abc de"), "abc de");
}

TEST(Optional, parseOptionalNull) {
    EXPECT_FALSE(*JutchsON::parseOptional("null"));
}

TEST(Optional, parseOptionalNullCamelCase) {
    EXPECT_FALSE(*JutchsON::parseOptional("Null"));
}

TEST(Optional, parseOptionalNullCapsLock) {
    EXPECT_FALSE(*JutchsON::parseOptional("NULL"));
}

TEST(Optional, parseOptionalAngleBrackets) {
    EXPECT_EQ(**JutchsON::parseOptional("<abc de>"), "abc de");
}

TEST(Optional, parseOptionalAngleBracketsNull) {
    EXPECT_EQ(**JutchsON::parseOptional("<null>"), "null");
}

TEST(Optional, parseOptionalEmpty) {
    EXPECT_EQ(**JutchsON::parseOptional(""), "");
}

TEST(Optional, parseStdOpitonal) {
    EXPECT_EQ(**JutchsON::parse<std::optional<int>>("123"), 123);
}

TEST(Optional, parseStdOpitonalNull) {
    EXPECT_FALSE(*JutchsON::parse<std::optional<int>>("null"));
}

TEST(Optional, parseStdOpitonalAngleBrackets) {
    EXPECT_EQ(**JutchsON::parse<std::optional<int>>("<123>"), 123);
}

TEST(Optional, parseStdOpitonalNested) {
    EXPECT_EQ(***JutchsON::parse<std::optional<std::optional<int>>>("123"), 123);
}

TEST(Optional, parseStdOpitonalNestedAngleBrackets) {
    EXPECT_EQ(***JutchsON::parse<std::optional<std::optional<int>>>("<123>"), 123);
}

TEST(Optional, parseStdOpitonalNestedDoubleAngleBrackets) {
    EXPECT_EQ(***JutchsON::parse<std::optional<std::optional<int>>>("<<123>>"), 123);
}

TEST(Optional, parseStdOpitonalNestedNull) {
    EXPECT_FALSE(*JutchsON::parse<std::optional<std::optional<int>>>("null"));
}

TEST(Optional, parseStdOpitonalNestedAngleBracketsNull) {
    EXPECT_FALSE(**JutchsON::parse<std::optional<std::optional<int>>>("<null>"));
}

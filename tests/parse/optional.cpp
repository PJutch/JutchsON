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

TEST(Optional, parseOptionalChevrons) {
    EXPECT_EQ(**JutchsON::parseOptional("<abc de>"), "abc de");
}

TEST(Optional, parseOptionalChevronsNull) {
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

TEST(Optional, parseStdOpitonalChevrons) {
    EXPECT_EQ(**JutchsON::parse<std::optional<int>>("<123>"), 123);
}

TEST(Optional, parseStdOpitonalNested) {
    EXPECT_EQ(***JutchsON::parse<std::optional<std::optional<int>>>("123"), 123);
}

TEST(Optional, parseStdOpitonalNestedChevrons) {
    EXPECT_EQ(***JutchsON::parse<std::optional<std::optional<int>>>("<123>"), 123);
}

TEST(Optional, parseStdOpitonalNestedDoubleChevrons) {
    EXPECT_EQ(***JutchsON::parse<std::optional<std::optional<int>>>("<<123>>"), 123);
}

TEST(Optional, parseStdOpitonalNestedNull) {
    EXPECT_FALSE(*JutchsON::parse<std::optional<std::optional<int>>>("null"));
}

TEST(Optional, parseStdOpitonalNestedChevronsNull) {
    EXPECT_FALSE(**JutchsON::parse<std::optional<std::optional<int>>>("<null>"));
}

TEST(Optional, parseStdOpitonalChevronsContext) {
    EXPECT_EQ(**JutchsON::parse<std::optional<std::vector<int>>>("<1 2 3>"), (std::vector<int>{1, 2, 3}));
}

namespace {
    struct TestType {};
    struct TestEnv {};
}

namespace JutchsON {
    template <>
    struct Parser<TestType> {
        ParseResult<TestType> operator() (StringView, TestEnv, Context) {
            return {{}};
        }
    };
}

TEST(Optional, parseStdOpitonalEnv) {
    EXPECT_TRUE(JutchsON::parse<std::optional<TestType>>("", TestEnv{}));
}

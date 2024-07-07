#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Tuple, parseTuple) {
    EXPECT_EQ((JutchsON::parse<std::tuple<int, int>>("1 2")), (std::tuple{1, 2}));
}

TEST(Tuple, parseTupleHeterogeneous) {
    EXPECT_EQ((JutchsON::parse<std::tuple<int, std::string>>("1 abc")), (std::tuple<int, std::string>{1, "abc"}));
}

TEST(Tuple, parseTupleWrongSize) {
    EXPECT_EQ((JutchsON::parse<std::tuple<int, int>>("1 2 3")),
        (JutchsON::ParseResult<std::tuple<int, int>>::makeError({0, 0}, "Tuple size should be 2 but it is 3")));
}

TEST(Tuple, parseTupleSingleton) {
    EXPECT_EQ(JutchsON::parse<std::tuple<int>>("1"), std::tuple{1});
}

TEST(Tuple, parseTupleEmpty) {
    EXPECT_TRUE(JutchsON::parse<std::tuple<>>(""));
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

TEST(Tuple, parseTupleEnv) {
    EXPECT_TRUE((JutchsON::parse<std::tuple<TestType, TestType>>("a b", TestEnv{})));
}

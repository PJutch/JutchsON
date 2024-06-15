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

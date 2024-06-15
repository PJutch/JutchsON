#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Tuple, writeTupleInt) {
    EXPECT_EQ(JutchsON::write(std::tuple{1, 2, 3}), "1 2 3");
}

TEST(Tuple, writeTupleHeterogeneous) {
    EXPECT_EQ(JutchsON::write(std::tuple<int, std::string>{1, "abcdefg"}), "1 abcdefg");
}

TEST(Tuple, writeTupleSingleton) {
    EXPECT_EQ(JutchsON::write(std::tuple<int>{1}), "1");
}

TEST(Tuple, writeTupleEmpty) {
    EXPECT_EQ(JutchsON::write(std::tuple<>{}), "");
}

TEST(Tuple, writeTupleNested) {
    EXPECT_EQ(JutchsON::write(std::tuple<int, std::tuple<int, int>>{1, {2, 3}}), "[\n    1\n    2 3\n]");
}

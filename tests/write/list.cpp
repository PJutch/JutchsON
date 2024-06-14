#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(List, writeOnelineList) {
    std::vector<std::string> data{"ab", "cde", "j"};
    EXPECT_EQ(JutchsON::writeOnelineList(data), "ab cde j");
}

TEST(List, writeOnelineListEmpty) {
    std::vector<std::string> data{};
    EXPECT_EQ(JutchsON::writeOnelineList(data), "");
}

TEST(List, writeOnelineListSingleton) {
    std::vector<std::string> data{"abc"};
    EXPECT_EQ(JutchsON::writeOnelineList(data), "abc");
}

TEST(List, writeOnelineListQuoted) {
    std::vector<std::string> data{"ab", "cde", "j"};
    EXPECT_EQ(JutchsON::writeOnelineList(data, true), "[ab cde j]");
}

TEST(List, writeOnelineListQuotedEmpty) {
    std::vector<std::string> data{};
    EXPECT_EQ(JutchsON::writeOnelineList(data, true), "[]");
}

TEST(List, writeOnelineListQuotedSingleton) {
    std::vector<std::string> data{"abc"};
    EXPECT_EQ(JutchsON::writeOnelineList(data, true), "[abc]");
}

TEST(List, writeMultilineList) {
    std::vector<std::string> data{"abc", "de"};
    EXPECT_EQ(JutchsON::writeMultilineList(data), "[\n    abc\n    de\n]");
}

TEST(List, writeMultilineListSingleton) {
    std::vector<std::string> data{"abc"};
    EXPECT_EQ(JutchsON::writeMultilineList(data), "[\n    abc\n]");
}

TEST(List, writeMultilineListEmpty) {
    std::vector<std::string> data{};
    EXPECT_EQ(JutchsON::writeMultilineList(data), "[]");
}

TEST(List, shouldBeMultilineType) {
    EXPECT_TRUE(JutchsON::shouldBeMultiline(std::vector{std::vector{1, 2}, std::vector{3, 4}}));
}

TEST(List, shouldBeMultilineSize) {
    EXPECT_TRUE(JutchsON::shouldBeMultiline(std::vector<int>(1000)));
}

TEST(List, shouldBeMultilineFalse) {
    EXPECT_FALSE(JutchsON::shouldBeMultiline(std::vector{1, 2, 3}));
}

TEST(List, writeVectorInt) {
    EXPECT_EQ(JutchsON::write(std::vector{1, 2, 3}), "1 2 3");
}

TEST(List, writeVectorString) {
    EXPECT_EQ(JutchsON::write(std::vector<std::string>{"abc", "de"}), "abc de");
}

TEST(List, writeVectorQuoted) {
    EXPECT_EQ(JutchsON::write(std::vector{1, 2, 3}, JutchsON::Context::OBJECT), "[1 2 3]");
}

TEST(List, writeVectorVector) {
    EXPECT_EQ(JutchsON::write(std::vector{std::vector{1, 2}, std::vector{3, 4}}), "[\n    1 2\n    3 4\n]");
}

TEST(List, writeTupleInt) {
    EXPECT_EQ(JutchsON::write(std::tuple{1, 2, 3}), "1 2 3");
}

TEST(List, writeTupleHeterogeneous) {
    EXPECT_EQ(JutchsON::write(std::tuple<int, std::string>{1, "abcdefg"}), "1 abcdefg");
}

TEST(List, writeTupleSingleton) {
    EXPECT_EQ(JutchsON::write(std::tuple<int>{1}), "1");
}

TEST(List, writeTupleEmpty) {
    EXPECT_EQ(JutchsON::write(std::tuple<>{}), "");
}

TEST(List, writeTupleNested) {
    EXPECT_EQ(JutchsON::write(std::tuple<int, std::tuple<int, int>>{1, {2, 3}}), "[\n    1\n    2 3\n]");
}

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

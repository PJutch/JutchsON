#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(List, writeOnelineList) {
    std::vector<std::string> data{"ab", "cde", "j"};
    EXPECT_EQ(JutchsON::writeList(data), "ab cde j");
}

TEST(List, writeOnelineListEmpty) {
    std::vector<std::string> data{};
    EXPECT_EQ(JutchsON::writeList(data), "");
}

TEST(List, writeOnelineListSingleton) {
    std::vector<std::string> data{"abc"};
    EXPECT_EQ(JutchsON::writeList(data), "abc");
}

TEST(List, writeOnelineListQuoted) {
    std::vector<std::string> data{"ab", "cde", "j"};
    EXPECT_EQ(JutchsON::writeList(data, true), "[ab cde j]");
}

TEST(List, writeOnelineListQuotedEmpty) {
    std::vector<std::string> data{};
    EXPECT_EQ(JutchsON::writeList(data, true), "[]");
}

TEST(List, writeOnelineListQuotedSingleton) {
    std::vector<std::string> data{"abc"};
    EXPECT_EQ(JutchsON::writeList(data, true), "[abc]");
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

TEST(List, writeList) {
    std::vector<std::string> data{"ab", "cde", "j"};
    EXPECT_EQ(JutchsON::writeList(data), "ab cde j");
}

TEST(List, writeListMultiline) {
    std::vector<std::string> data{"[ab\ncde]", "j"};
    EXPECT_EQ(JutchsON::writeList(data), "[\n    [ab\n    cde]\n    j\n]");
}

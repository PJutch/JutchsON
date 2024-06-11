#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(List, parseList) {
    EXPECT_EQ(JutchsON::parseList("ab\t cde j"), (std::vector<JutchsON::StringView>{"ab", "cde", "j"}));
}

TEST(List, parseListFrontSpaces) {
    EXPECT_EQ(JutchsON::parseList("  ab\t cde j"), (std::vector<JutchsON::StringView>{"ab", "cde", "j"}));
}

TEST(List, parseListBackSpace) {
    EXPECT_EQ(JutchsON::parseList("ab\t cde j  "), (std::vector<JutchsON::StringView>{"ab", "cde", "j"}));
}

TEST(List, parseListBothSpaces) {
    EXPECT_EQ(JutchsON::parseList("   ab\t cde j  "), (std::vector<JutchsON::StringView>{"ab", "cde", "j"}));
}

TEST(List, parseListSingle) {
    EXPECT_EQ(JutchsON::parseList("abc"), (std::vector<JutchsON::StringView>{"abc"}));
}

TEST(List, parseListOnlySpaces) {
    EXPECT_TRUE(JutchsON::parseList("    ")->empty());
}

TEST(List, parseListEmpty) {
    EXPECT_TRUE(JutchsON::parseList("")->empty());
}

TEST(List, parseListNested) {
    EXPECT_EQ(JutchsON::parseList("ab\t [cde wxyz] j"), (std::vector<JutchsON::StringView>{"ab", "[cde wxyz]", "j"}));
}

TEST(List, parseListNestedNoFrontSpace) {
    EXPECT_EQ(JutchsON::parseList("ab[cde wxyz] j"), (std::vector<JutchsON::StringView>{"ab", "[cde wxyz]", "j"}));
}

TEST(List, parseListNestedNoBackSpace) {
    EXPECT_EQ(JutchsON::parseList("ab\t [cde wxyz]j"), (std::vector<JutchsON::StringView>{"ab", "[cde wxyz]", "j"}));
}

TEST(List, parseListNestedNoBothSpaces) {
    EXPECT_EQ(JutchsON::parseList("ab[cde wxyz]j"), (std::vector<JutchsON::StringView>{"ab", "[cde wxyz]", "j"}));
}

TEST(List, parseListBrackets) {
    EXPECT_EQ(JutchsON::parseList("[ab\t cde j]"), (std::vector<JutchsON::StringView>{"ab", "cde", "j"}));
}

TEST(List, parseListEmptyBrackets) {
    EXPECT_TRUE(JutchsON::parseList("[]")->empty());
}

TEST(List, parseListUnmatchedOpeningBracket) {
    EXPECT_EQ(JutchsON::parseList("[abc de"), JutchsON::ParseResult<std::vector<JutchsON::StringView>>::makeError({0, 0}, "Unmatched ["));
}

TEST(List, parseListUnmatchedClosingBracket) {
    EXPECT_EQ(JutchsON::parseList("abc de]"), JutchsON::ParseResult<std::vector<JutchsON::StringView>>::makeError({0, 6}, "Unmatched ]"));
}

TEST(List, parseListMultiline) {
    EXPECT_EQ(JutchsON::parseList("ab\n cde wxyz"), (std::vector<JutchsON::StringView>{"ab", "cde wxyz"}));
}

TEST(List, parseListMultilineSpace) {
    EXPECT_EQ(JutchsON::parseList("ab \n cde wxyz"), (std::vector<JutchsON::StringView>{"ab", "cde wxyz"}));
}

TEST(List, parseListMultilineBrackets) {
    EXPECT_EQ(JutchsON::parseList("[ab\n cde wxyz]"), (std::vector<JutchsON::StringView>{"ab", "cde wxyz"}));
}

TEST(List, parseListMultilineSeparateBrackets) {
    EXPECT_EQ(JutchsON::parseList("[\nab cde wxyz\n]"), (std::vector<JutchsON::StringView>{"ab", "cde", "wxyz"}));
}

TEST(List, parseListMultilineDoubleNewline) {
    EXPECT_EQ(JutchsON::parseList("ab cde wxyz\n\nj"), (std::vector<JutchsON::StringView>{"ab cde wxyz", "j"}));
}

TEST(List, parseListMultilineEmpty) {
    EXPECT_TRUE(JutchsON::parseList("\n\n\n")->empty());
}

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

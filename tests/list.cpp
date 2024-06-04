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

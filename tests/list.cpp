#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(List, findObjectBegin) {
    std::string_view s = "ab\t cde j";
    EXPECT_EQ(findObjectBegin(s), 0);
}

TEST(List, findObjectBeginSpace) {
    std::string_view s = "  \n ab  cde j";
    EXPECT_EQ(findObjectBegin(s), 4);
}

TEST(List, findObjectBeginSpaceOnly) {
    std::string_view s = "  \n \t  ";
    EXPECT_EQ(findObjectBegin(s), std::ssize(s));
}

TEST(List, findObjectBeginEmpty) {
    std::string_view s = "";
    EXPECT_EQ(findObjectBegin(s), 0);
}

TEST(List, findObjectEnd) {
    std::string_view s = "ab\t cde j";
    EXPECT_EQ(findObjectEnd(s), 2);
}

TEST(List, findObjectEndSpace) {
    std::string_view s = "  \n ab  cde j";
    EXPECT_EQ(findObjectEnd(s), 0);
}

TEST(List, findObjectEndSpaceOnly) {
    std::string_view s = "  \n \t  ";
    EXPECT_EQ(findObjectEnd(s), 0);
}

TEST(List, findObjectEndEmpty) {
    std::string_view s = "";
    EXPECT_EQ(findObjectEnd(s), 0);
}

TEST(List, findObjectEndEnd) {
    std::string_view s = "abcdej";
    EXPECT_EQ(findObjectEnd(s), std::ssize(s));
}

TEST(List, parseList) {
    EXPECT_EQ(parseList("ab\t cde j"), (std::vector<std::string_view>{"ab", "cde", "j"}));
}

TEST(List, parseListFrontSpaces) {
    EXPECT_EQ(parseList("  ab\t cde j"), (std::vector<std::string_view>{"ab", "cde", "j"}));
}

TEST(List, parseListBackSpace) {
    EXPECT_EQ(parseList("ab\t cde j  "), (std::vector<std::string_view>{"ab", "cde", "j"}));
}

TEST(List, parseListBothSpaces) {
    EXPECT_EQ(parseList("   ab\t cde j  "), (std::vector<std::string_view>{"ab", "cde", "j"}));
}

TEST(List, parseListSingle) {
    EXPECT_EQ(parseList("abc"), (std::vector<std::string_view>{"abc"}));
}

TEST(List, parseListOnlySpaces) {
    EXPECT_TRUE(parseList("    ").getOk().empty());
}

TEST(List, parseListEmpty) {
    EXPECT_TRUE(parseList("").getOk().empty());
}

TEST(List, parseListNested) {
    EXPECT_EQ(parseList("ab\t [cde wxyz] j"), (std::vector<std::string_view>{"ab", "[cde wxyz]", "j"}));
}

TEST(List, parseListNestedNoFrontSpace) {
    EXPECT_EQ(parseList("ab[cde wxyz] j"), (std::vector<std::string_view>{"ab", "[cde wxyz]", "j"}));
}

TEST(List, parseListNestedNoBackSpace) {
    EXPECT_EQ(parseList("ab\t [cde wxyz]j"), (std::vector<std::string_view>{"ab", "[cde wxyz]", "j"}));
}

TEST(List, parseListNestedNoBothSpaces) {
    EXPECT_EQ(parseList("ab[cde wxyz]j"), (std::vector<std::string_view>{"ab", "[cde wxyz]", "j"}));
}

TEST(List, parseListBrackets) {
    EXPECT_EQ(parseList("[ab\t cde j]"), (std::vector<std::string_view>{"ab", "cde", "j"}));
}

TEST(List, parseListEmptyBrackets) {
    EXPECT_TRUE(parseList("[]").getOk().empty());
}

TEST(List, parseListUnmatchedOpeningBracket) {
    EXPECT_EQ(parseList("[abc de"), JutchsON::ParseResult<std::vector<std::string_view>>::makeError({0, 0}, "Unmatched ["));
}

TEST(List, parseListUnmatchedClosingBracket) {
    EXPECT_EQ(parseList("abc de]"), JutchsON::ParseResult<std::vector<std::string_view>>::makeError({0, 6}, "Unmatched ]"));
}

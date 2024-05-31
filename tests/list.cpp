#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(List, findObjectBegin) {
    std::string_view s = "ab\t cde j";
    EXPECT_EQ(JutchsON::findObjectBegin(s), 0);
}

TEST(List, findObjectBeginSpace) {
    std::string_view s = "  \n ab  cde j";
    EXPECT_EQ(JutchsON::findObjectBegin(s), 4);
}

TEST(List, findObjectBeginSpaceOnly) {
    std::string_view s = "  \n \t  ";
    EXPECT_EQ(JutchsON::findObjectBegin(s), std::ssize(s));
}

TEST(List, findObjectBeginEmpty) {
    std::string_view s = "";
    EXPECT_EQ(JutchsON::findObjectBegin(s), 0);
}

TEST(List, findObjectEnd) {
    std::string_view s = "ab\t cde j";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 2);
}

TEST(List, findObjectEndSpace) {
    std::string_view s = "  \n ab  cde j";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 0);
}

TEST(List, findObjectEndSpaceOnly) {
    std::string_view s = "  \n \t  ";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 0);
}

TEST(List, findObjectEndEmpty) {
    std::string_view s = "";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 0);
}

TEST(List, findObjectEndEnd) {
    std::string_view s = "abcdej";
    EXPECT_EQ(JutchsON::findObjectEnd(s), std::ssize(s));
}

TEST(List, findLineObjectEnd) {
    std::string_view s = "ab \n cde j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 3);
}

TEST(List, findLineObjectEnd1Line) {
    std::string_view s = "  ab  cde j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), std::ssize(s));
}

TEST(List, findLineObjectEndFront) {
    std::string_view s = "\nabc";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 0);
}

TEST(List, findLineObjectEndEmpty) {
    std::string_view s = "";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 0);
}

TEST(List, findLineObjectNewlineInList) {
    std::string_view s = "ab [ce \nwxyz] cd\n j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 16);
}

TEST(List, findLineObjectNoNewlineInList) {
    std::string_view s = "ab [ce wxyz] cd\n j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 15);
}

TEST(List, findLineObjectListOnly) {
    std::string_view s = "[ab ce\n wxyz cd\n j]";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), std::ssize(s));
}

TEST(List, isMulitiline) {
    EXPECT_TRUE(JutchsON::isMultiline("ab ce\n wxyz cd\n j"));
}

TEST(List, isMulitilineFalse) {
    EXPECT_FALSE(JutchsON::isMultiline("ab ce j"));
}

TEST(List, isMulitilineInBrackets) {
    EXPECT_FALSE(JutchsON::isMultiline("ab [cd\n wxyz] j"));
}

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
    EXPECT_TRUE(JutchsON::parseList("    ").getOk().empty());
}

TEST(List, parseListEmpty) {
    EXPECT_TRUE(JutchsON::parseList("").getOk().empty());
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
    EXPECT_TRUE(JutchsON::parseList("[]").getOk().empty());
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
    EXPECT_TRUE(JutchsON::parseList("\n\n\n").getOk().empty());
}

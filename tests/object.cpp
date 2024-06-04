#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Object, findObjectBegin) {
    std::string_view s = "ab\t cde j";
    EXPECT_EQ(JutchsON::findObjectBegin(s), 0);
}

TEST(Object, findObjectBeginSpace) {
    std::string_view s = "  \n ab  cde j";
    EXPECT_EQ(JutchsON::findObjectBegin(s), 4);
}

TEST(Object, findObjectBeginSpaceOnly) {
    std::string_view s = "  \n \t  ";
    EXPECT_EQ(JutchsON::findObjectBegin(s), std::ssize(s));
}

TEST(Object, findObjectBeginEmpty) {
    std::string_view s = "";
    EXPECT_EQ(JutchsON::findObjectBegin(s), 0);
}

TEST(Object, findObjectEnd) {
    std::string_view s = "ab\t cde j";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 2);
}

TEST(Object, findObjectEndSpace) {
    std::string_view s = "  \n ab  cde j";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 0);
}

TEST(Object, findObjectEndSpaceOnly) {
    std::string_view s = "  \n \t  ";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 0);
}

TEST(Object, findObjectEndEmpty) {
    std::string_view s = "";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 0);
}

TEST(Object, findObjectEndEnd) {
    std::string_view s = "abcdej";
    EXPECT_EQ(JutchsON::findObjectEnd(s), std::ssize(s));
}

TEST(Object, findObjectEndAllInBrackets) {
    std::string_view s = "[abcdej]";
    EXPECT_EQ(JutchsON::findObjectEnd(s), std::ssize(s));
}

TEST(Object, findObjectEndBrackets) {
    std::string_view s = "[abc de j]  fgh\n ";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 10);
}

TEST(Object, findObjectEndNestedBrackets) {
    std::string_view s = "[abc [de j] xyz]  fgh\n ";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 16);
}

TEST(Object, findObjectEndNoBracket) {
    std::string_view s = "[abc ij xyz";
    JutchsON::Location location{0, std::ssize(s)};
    EXPECT_EQ(JutchsON::findObjectEnd(s), JutchsON::ParseResult<ptrdiff_t>::makeError(location, "Expected ], got eof"));
}

TEST(Object, findObjectEndWrongBracket) {
    std::string_view s = "{abc ij] xyz";
    EXPECT_EQ(JutchsON::findObjectEnd(s), JutchsON::ParseResult<ptrdiff_t>::makeError(JutchsON::Location{0, 7}, "Expected }, got ]"));
}

TEST(Object, findObjectEndNoBrace) {
    std::string_view s = "{abc ij xyz";
    JutchsON::Location location{0, std::ssize(s)};
    EXPECT_EQ(JutchsON::findObjectEnd(s), JutchsON::ParseResult<ptrdiff_t>::makeError(location, "Expected }, got eof"));
}

TEST(Object, findObjectEndWrongBrace) {
    std::string_view s = "[abc ij} xyz";
    EXPECT_EQ(JutchsON::findObjectEnd(s), JutchsON::ParseResult<ptrdiff_t>::makeError(JutchsON::Location{0, 7}, "Expected ], got }"));
}

TEST(Object, findObjectEndBraces) {
    std::string_view s = "{abc de j}  fgh\n ";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 10);
}

TEST(Object, findObjectEndNestedBraces) {
    std::string_view s = "{abc {de j} xyz}  fgh\n ";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 16);
}

TEST(Object, findObjectEndNestedDifferent) {
    std::string_view s = "[abc {de j} xyz]  fgh\n ";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 16);
}

TEST(Object, findObjectEndBracket) {
    std::string_view s = "fgh[abcxy]";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 3);
}

TEST(Object, findObjectEndBrace) {
    std::string_view s = "fgh{abcxy}";
    EXPECT_EQ(JutchsON::findObjectEnd(s), 3);
}

TEST(Object, findLineObjectEnd) {
    std::string_view s = "ab \n cde j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 3);
}

TEST(Object, findLineObjectEnd1Line) {
    std::string_view s = "  ab  cde j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), std::ssize(s));
}

TEST(Object, findLineObjectEndFront) {
    std::string_view s = "\nabc";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 0);
}

TEST(Object, findLineObjectEndEmpty) {
    std::string_view s = "";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 0);
}

TEST(Object, findLineObjectNewlineInBrackets) {
    std::string_view s = "ab [ce \nwxyz] cd\n j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 16);
}

TEST(Object, findLineObjectNewlineInNestedBrackets) {
    std::string_view s = "ab [ce[ \nw]xyz] cd\n j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 18);
}

TEST(Object, findLineObjectNewlineInBraces) {
    std::string_view s = "ab {ce \nwxyz} cd\n j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 16);
}

TEST(Object, findLineObjectNewlineInNestedBraces) {
    std::string_view s = "ab {ce{ \nw}xyz} cd\n j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 18);
}

TEST(Object, findLineObjectNewlineInNestedDifferent) {
    std::string_view s = "ab [ce{ \nw}xyz] cd\n j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 18);
}

TEST(Object, findLineObjectNoNewlineInBrackets) {
    std::string_view s = "ab [ce wxyz] cd\n j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 15);
}

TEST(Object, findLineObjectAllInBrackets) {
    std::string_view s = "[ab ce\n wxyz cd\n j]";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), std::ssize(s));
}

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

TEST(Object, findLineObjectNewlineInList) {
    std::string_view s = "ab [ce \nwxyz] cd\n j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 16);
}

TEST(Object, findLineObjectNoNewlineInList) {
    std::string_view s = "ab [ce wxyz] cd\n j";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), 15);
}

TEST(Object, findLineObjectListOnly) {
    std::string_view s = "[ab ce\n wxyz cd\n j]";
    EXPECT_EQ(JutchsON::findLineObjectEnd(s), std::ssize(s));
}
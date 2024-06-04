#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Dict, parseDict) {
    std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>> result{{"abc", "def"}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::parseDict("abc def gh ij"), result);
}

TEST(Dict, parseDictBigSpaces) {
    std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>> result{{"abc", "def"}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::parseDict("abc  def \tgh    ij\n"), result);
}

TEST(Dict, parseDictNoValue) {
    JutchsON::StringView s = "abc def gh";
    auto result = JutchsON::ParseResult<std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>>>
        ::makeError({0, std::ssize(s)}, "No value given for key");
    EXPECT_EQ(JutchsON::parseDict(s), result);
}

TEST(Dict, parseDictBraces) {
    std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>> result{{"abc", "def"}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::parseDict("{abc def gh ij}"), result);
}

TEST(Dict, parseDictMultiline) {
    std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>> result{{"abc", "def xyz"}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::parseDict("abc def xyz\ngh ij"), result);
}

TEST(Dict, parseDictMultilineBigSpaces) {
    std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>> result{{"abc", "def xyz"}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::parseDict("\nabc  def xyz   \ngh\t ij\n"), result);
}

TEST(Dict, parseDictMultilineBraces) {
    std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>> result{{"abc", "def xyz"}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::parseDict("{abc def xyz\ngh ij}"), result);
}

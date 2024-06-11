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

TEST(Dict, parseDictMultilineNoValue) {
    std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>> result{{"abc", ""}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::parseDict("abc\ngh ij"), result);
}

TEST(Dict, parseDictMultilineNoValueSpace) {
    std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>> result{{"abc", ""}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::parseDict("abc  \ngh ij"), result);
}

TEST(Dict, parseDictMultilineBraces) {
    std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>> result{{"abc", "def xyz"}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::parseDict("{abc def xyz\ngh ij}"), result);
}

TEST(Dict, writeDict) {
    std::vector<std::pair<std::string, std::string>> data{{"abc", "def xyz"}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::writeDict(data), "{\n    abc def xyz\n    gh ij\n}");
}

TEST(Dict, writeDictEmptyValue) {
    std::vector<std::pair<std::string, std::string>> data{{"abc", ""}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::writeDict(data), "{\n    abc\n    gh ij\n}");
}

TEST(Dict, writeDictSingleton) {
    std::vector<std::pair<std::string, std::string>> data{{"abc", "def xyz"}};
    EXPECT_EQ(JutchsON::writeDict(data), "{\n    abc def xyz\n}");
}

TEST(Dict, writeDictEmpty) {
    std::vector<std::pair<std::string, std::string>> data{};
    EXPECT_EQ(JutchsON::writeDict(data), "{}");
}

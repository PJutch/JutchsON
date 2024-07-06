#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Str, parseStr) {
    EXPECT_EQ(JutchsON::parseStr("abcde"), "abcde");
}

TEST(Str, parseStrQuotes) {
    EXPECT_EQ(JutchsON::parseStr("\"abcde\""), "abcde");
}

TEST(Str, parseStrUnmatchedQuoteBegin) {
    EXPECT_EQ(JutchsON::parseStr("\"abcde"), JutchsON::ParseResult<std::string>::makeError(JutchsON::Location{0, 0}, "Unmatched \""));
}

TEST(Str, parseStrUnmatchedQuoteEnd) {
    EXPECT_EQ(JutchsON::parseStr("abcde\""), JutchsON::ParseResult<std::string>::makeError(JutchsON::Location{0, 5}, "Unmatched \""));
}

TEST(Str, parseStrEscapeSeq) {
    EXPECT_EQ(JutchsON::parseStr("abc\\nde"), "abc\nde");
}

TEST(Str, parseStrTruncatedEscapeSeq) {
    EXPECT_EQ(JutchsON::parseStr("abcdef\\"),
        JutchsON::ParseResult<std::string>::makeError(JutchsON::Location{0, 7}, "Truncated escape sequence"));
}

TEST(Str, parseStrEscapedQuote) {
    EXPECT_EQ(JutchsON::parseStr("\"abc\\\"d\""), "abc\"d");
}

TEST(Str, parseStrUnmatchedEscapedQuote) {
    EXPECT_EQ(JutchsON::parseStr("\"abcd\\\""), JutchsON::ParseResult<std::string>::makeError(JutchsON::Location{0, 0}, "Unmatched \""));
}

TEST(Str, parseStrEscapedSlash) {
    EXPECT_EQ(JutchsON::parseStr("\"abcd\\\\\""), "abcd\\");
}

TEST(Str, parseStrHexEscape) {
    EXPECT_EQ(JutchsON::parseStr("abcd\\x2a"), "abcd\x2a");
}

TEST(Str, parseStrHexEscapeTruncated) {
    EXPECT_EQ(JutchsON::parseStr("abcd\\x"),
        JutchsON::ParseResult<std::string>::makeError(JutchsON::Location{0, 6}, "Truncated \\x escape sequence"));
}

TEST(Str, parseStrHexEscapeNotADigit) {
    EXPECT_EQ(JutchsON::parseStr("abcd\\xgg"),
        JutchsON::ParseResult<std::string>::makeError(JutchsON::Location{0, 6}, "'g' is not a hex digit"));
}

TEST(Str, parseStrHexEscapeQouteNotADigit) {
    EXPECT_EQ(JutchsON::parseStr("\"abcd\\x\""),
        JutchsON::ParseResult<std::string>::makeError(JutchsON::Location{0, 7}, "'\"' is not a hex digit"));
}

TEST(Str, parseStrSpaces) {
    EXPECT_EQ(JutchsON::parseStr("abc de"), "abc de");
}

TEST(Str, parseGenericString) {
    EXPECT_EQ(JutchsON::parse<std::string>("abc de"), "abc de");
}

TEST(Str, parsePath) {
    EXPECT_EQ(JutchsON::parse<std::filesystem::path>("abc/de"), "abc/de");
}

#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Str, writeStr) {
    EXPECT_EQ(JutchsON::writeStr("abcde"), "abcde");
}

TEST(Str, writeStrEscape) {
    EXPECT_EQ(JutchsON::writeStr("abc\nde"), "abc\\nde");
}

TEST(Str, writeStrQuote) {
    EXPECT_EQ(JutchsON::writeStr("abc de"), "\"abc de\"");
}

TEST(Str, writeStrQuoteQuotes) {
    EXPECT_EQ(JutchsON::writeStr("abc\"de"), "\"abc\\\"de\"");
}

TEST(Str, writeStrQuoteOpeningBracket) {
    EXPECT_EQ(JutchsON::writeStr("abc[de"), "\"abc[de\"");
}

TEST(Str, writeStrQuoteClosingBracket) {
    EXPECT_EQ(JutchsON::writeStr("abc]de"), "\"abc]de\"");
}

TEST(Str, writeStrQuoteOpeningBrace) {
    EXPECT_EQ(JutchsON::writeStr("abc{de"), "\"abc{de\"");
}

TEST(Str, writeStrQuoteClosingBrace) {
    EXPECT_EQ(JutchsON::writeStr("abc}de"), "\"abc}de\"");
}

TEST(Str, writeStrUnquoted) {
    EXPECT_EQ(JutchsON::writeStr("abc de", false), "abc de");
}

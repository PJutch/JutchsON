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
    JutchsON::StringView s = "[abc de";
    EXPECT_EQ(JutchsON::parseList(s), 
        JutchsON::ParseResult<std::vector<JutchsON::StringView>>::makeError({0, std::ssize(s)}, "Expected ], got eof"));
}

TEST(List, parseListUnmatchedClosingBracket) {
    JutchsON::StringView s = "abc de]";
    EXPECT_EQ(JutchsON::parseList(s), 
        JutchsON::ParseResult<std::vector<JutchsON::StringView>>::makeError({0, std::ssize(s) - 1}, "Unexpected ]"));
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

TEST(List, parseList2D) {
    EXPECT_EQ(JutchsON::parseList("[1 2] [3 4]"), (std::vector<JutchsON::StringView>{"[1 2]", "[3 4]"}));
}

TEST(List, parseListFirst) {
    EXPECT_EQ(JutchsON::parseList("[1 2] xyz"), (std::vector<JutchsON::StringView>{"[1 2]", "xyz"}));
}

TEST(List, parseListLast) {
    EXPECT_EQ(JutchsON::parseList("xyz [3 4]"), (std::vector<JutchsON::StringView>{"xyz", "[3 4]"}));
}

TEST(List, parseVectorInt) {
    EXPECT_EQ(JutchsON::parse<std::vector<int>>("1 2 3"), (std::vector{1, 2, 3}));
}

TEST(List, parseVectorErrorCollection) {
    EXPECT_EQ(JutchsON::parse<std::vector<int>>("g 2 h"), (JutchsON::ParseResult<std::vector<int>>{std::vector<JutchsON::ParseError>{
        {{0, 0}, "Number can't contain char 'g'"}, {{0, 4}, "Number can't contain char 'h'"}
    }}));
}

TEST(List, parseVectorString) {
    EXPECT_EQ(JutchsON::parse<std::vector<std::string>>("abc de"), (std::vector<std::string>{"abc", "de"}));
}

TEST(List, parseVectorVectorInt) {
    EXPECT_EQ(JutchsON::parse<std::vector<std::vector<int>>>("[1 2] [3 4]"), (std::vector<std::vector<int>>{{1, 2}, {3, 4}}));
}

TEST(List, parseVectorVectorErrorCollection) {
    using ParsedT = std::vector<std::vector<int>>;
    EXPECT_EQ(JutchsON::parse<ParsedT>("[g 2] [h 4]"), (JutchsON::ParseResult<ParsedT>{std::vector<JutchsON::ParseError>{
        {{0, 1}, "Number can't contain char 'g'"}, {{0, 7}, "Number can't contain char 'h'"}
    }}));
}

TEST(List, parseVectorVectorNotAList) {
    using ParsedT = std::vector<std::vector<int>>;
    EXPECT_EQ(JutchsON::parse<ParsedT>("garbage [3 4]", JutchsON::Context::OBJECT),
        JutchsON::ParseResult<ParsedT>::makeError({0, 0}, "Expected a nested list")
    );
}

TEST(List, parseTuple) {
    EXPECT_EQ((JutchsON::parse<std::tuple<int, int>>("1 2")), (std::tuple{1, 2}));
}

TEST(List, parseTupleHeterogeneous) {
    EXPECT_EQ((JutchsON::parse<std::tuple<int, std::string>>("1 abc")), (std::tuple<int, std::string>{1, "abc"}));
}

TEST(List, parseTupleWrongSize) {
    EXPECT_EQ((JutchsON::parse<std::tuple<int, int>>("1 2 3")),
        (JutchsON::ParseResult<std::tuple<int, int>>::makeError({0, 0}, "Tuple size should be 2 but it is 3")));
}

TEST(List, parseTupleSingleton) {
    EXPECT_EQ(JutchsON::parse<std::tuple<int>>("1"), std::tuple{1});
}

TEST(List, parseTupleEmpty) {
    EXPECT_TRUE(JutchsON::parse<std::tuple<>>(""));
}

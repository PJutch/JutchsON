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

TEST(Dict, parseDictLast) {
    EXPECT_EQ(JutchsON::parseDict("xyz {3 4}"), 
        (std::vector<std::pair<JutchsON::StringView, JutchsON::StringView>>{{"xyz", "{3 4}"}}));
}

TEST(Dict, parseUnorderedMulimapInt) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<int, int>>("1 2\n3 4")), (std::unordered_multimap<int, int>{{1, 2}, {3, 4}}));
}

TEST(Dict, parseUnorderedMulimapIntOneline) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<int, int>>("1 2 3 4")), (std::unordered_multimap<int, int>{{1, 2}, {3, 4}}));
}

TEST(Dict, parseUnorderedMulimapString) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<std::string, std::string>>("abc de\nj xyz")),
		(std::unordered_multimap<std::string, std::string>{{"abc", "de"}, {"j", "xyz"}}));
}

TEST(Dict, parseUnorderedMulimapIntStringLineRest) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<int, std::string>>("1 ab c\n2 de")),
		(std::unordered_multimap<int, std::string>{{1, "ab c"}, {2, "de"}}));
}

TEST(Dict, parseUnorderedMulimapIntBool) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<int, bool>>("1\n2 false\n3 true")),
		(std::unordered_multimap<int, bool>{{1, true}, {2, false}, {3, true}}));
}

TEST(Dict, parseUnorderedMulimapIntNested) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<int, std::unordered_multimap<int, int>>>("1 {1 2\n 3 4}\n2 {5 6\n 7 8}")),
		(std::unordered_multimap<int, std::unordered_multimap<int, int>>{{1, {{1, 2}, {3, 4}}}, {2, {{5, 6}, {7, 8}}}}));
}

TEST(Dict, parseUnorderedMultiMapDirectory) {
    EXPECT_EQ((JutchsON::parseFile<std::unordered_multimap<std::string, int>>("../../../../tests/parse/files/dir")),
        (std::unordered_multimap<std::string, int>{{"ab", 123}, {"c", 456}, {"de", 789}}));
}

TEST(Dict, parseUnorderedMultiMapDirectoryIntKey) {
    EXPECT_EQ((JutchsON::parseFile<std::unordered_multimap<int, int>>("../../../../tests/parse/files/intNamesDir")),
        (std::unordered_multimap<int, int>{{1, 2}, {2, 5}}));
}

namespace {
    struct TestType {};
    struct TestEnv {};
}

namespace JutchsON {
    template <>
    struct Parser<TestType> {
        ParseResult<TestType> operator() (StringView s, TestEnv, Context) {
            return {{}};
        }
    };
}

TEST(Dict, parseUnorderedMultimapEnv) {
    EXPECT_TRUE((JutchsON::parse<std::unordered_multimap<int, TestType>>("1 2\n3 4\n5 6 7", TestEnv{})));
}

TEST(Dict, parseUnorderedMultimapEnvDirectory) {
    using T = std::unordered_multimap<std::string, TestType>;
    EXPECT_TRUE(JutchsON::parseFile<T>("../../../../tests/parse/files/dir", TestEnv{}));
}

TEST(Dict, parseUnorderedMapInt) {
	EXPECT_EQ((JutchsON::parse<std::unordered_map<int, int>>("1 2\n3 4")), (std::unordered_map<int, int>{{1, 2}, {3, 4}}));
}

TEST(Dict, parseUnorderedMapIntDuplicated) {
	EXPECT_EQ((JutchsON::parse<std::unordered_map<int, int>>("1 2\n1 4")),
		(JutchsON::ParseResult<std::unordered_map<int, int>>::makeError({0, 0}, "Duplicated key detected")));
}

TEST(Dict, parseUnorderedMapDirectory) {
    EXPECT_EQ((JutchsON::parseFile<std::unordered_map<std::string, int>>("../../../../tests/parse/files/dir")),
        (std::unordered_map<std::string, int>{{"ab", 123}, {"c", 456}, {"de", 789}}));
}

TEST(Dict, parseUnorderedMapEnv) {
    EXPECT_TRUE((JutchsON::parse<std::unordered_map<int, TestType>>("1 2\n3 4\n5 6 7", TestEnv{})));
}

TEST(Dict, parseUnorderedMapEnvDirectory) {
    using T = std::unordered_map<std::string, TestType>;
    EXPECT_TRUE(JutchsON::parseFile<T>("../../../../tests/parse/files/dir", TestEnv{}));
}

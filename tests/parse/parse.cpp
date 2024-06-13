#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Parse, parseUnsignedInt) {
	EXPECT_EQ(JutchsON::parse<unsigned int>("123"), 123u);
}

TEST(Parse, parseInt) {
	EXPECT_EQ(JutchsON::parse<int>("-123"), -123);
}

TEST(Parse, parseFloat) {
	EXPECT_DOUBLE_EQ(*JutchsON::parse<double>("1.23"), 1.23);
}

TEST(Parse, parseString) {
	EXPECT_EQ(JutchsON::parse<std::string>("abc de"), "abc de");
}

TEST(Parse, parseBool) {
	EXPECT_TRUE(*JutchsON::parse<bool>("true"));
}

TEST(Parse, parseBoolEmpty) {
	EXPECT_EQ(JutchsON::parse<bool>("", JutchsON::Context::LINE), 
		JutchsON::ParseResult<bool>::makeError({0, 0}, "Empty bool isn't allowed"));
}

TEST(Parse, parseBoolEmptyAllowed) {
	EXPECT_TRUE(*JutchsON::parse<bool>("", JutchsON::Context::LINE_REST));
}

TEST(Parse, parseVectorInt) {
	EXPECT_EQ(JutchsON::parse<std::vector<int>>("1 2 3"), (std::vector{1, 2, 3}));
}

TEST(Parse, parseVectorErrorCollection) {
	EXPECT_EQ(JutchsON::parse<std::vector<int>>("g 2 h"), (JutchsON::ParseResult<std::vector<int>>{std::vector<JutchsON::ParseError>{
		{{0, 0}, "Number can't contain char 'g'"}, {{0, 4}, "Number can't contain char 'h'"}
	}}));
}

TEST(Parse, parseVectorString) {
	EXPECT_EQ(JutchsON::parse<std::vector<std::string>>("abc de"), (std::vector<std::string>{"abc", "de"}));
}

TEST(Parse, parseVectorVectorInt) {
	EXPECT_EQ(JutchsON::parse<std::vector<std::vector<int>>>("[1 2] [3 4]"), (std::vector<std::vector<int>>{{1, 2}, {3, 4}}));
}

TEST(Parse, parseVectorVectorErrorCollection) {
	using ParsedT = std::vector<std::vector<int>>;
	EXPECT_EQ(JutchsON::parse<ParsedT>("[g 2] [h 4]"), (JutchsON::ParseResult<ParsedT>{std::vector<JutchsON::ParseError>{
		{{0, 1}, "Number can't contain char 'g'"}, {{0, 7}, "Number can't contain char 'h'"}
	}}));
}

TEST(Parse, parseVectorVectorNotAList) {
	using ParsedT = std::vector<std::vector<int>>;
	EXPECT_EQ(JutchsON::parse<ParsedT>("garbage [3 4]", JutchsON::Context::OBJECT),
		JutchsON::ParseResult<ParsedT>::makeError({0, 0}, "Expected a nested list")
	);
}

TEST(Parse, parseUnorderedMulimapInt) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<int, int>>("1 2\n3 4")), (std::unordered_multimap<int, int>{{1, 2}, {3, 4}}));
}

TEST(Parse, parseUnorderedMulimapIntOneline) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<int, int>>("1 2 3 4")), (std::unordered_multimap<int, int>{{1, 2}, {3, 4}}));
}

TEST(Parse, parseUnorderedMulimapString) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<std::string, std::string>>("abc de\nj xyz")), 
		(std::unordered_multimap<std::string, std::string>{{"abc", "de"}, {"j", "xyz"}}));
}

TEST(Parse, parseUnorderedMulimapIntStringLineRest) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<int, std::string>>("1 ab c\n2 de")), 
		(std::unordered_multimap<int, std::string>{{1, "ab c"}, {2, "de"}}));
}

TEST(Parse, parseUnorderedMulimapIntBool) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<int, bool>>("1\n2 false\n3 true")),
		(std::unordered_multimap<int, bool>{{1, true}, {2, false}, {3, true}}));
}

TEST(Parse, parseUnorderedMulimapIntNested) {
	EXPECT_EQ((JutchsON::parse<std::unordered_multimap<int, std::unordered_multimap<int, int>>>("1 {1 2\n 3 4}\n2 {5 6\n 7 8}")),
		(std::unordered_multimap<int, std::unordered_multimap<int, int>>{{1, {{1, 2}, {3, 4}}}, {2, {{5, 6}, {7, 8}}}}));
}

TEST(Parse, parseUnorderedMapInt) {
	EXPECT_EQ((JutchsON::parse<std::unordered_map<int, int>>("1 2\n3 4")), (std::unordered_map<int, int>{{1, 2}, {3, 4}}));
}

TEST(Parse, parseUnorderedMapIntDuplicated) {
	EXPECT_EQ((JutchsON::parse<std::unordered_map<int, int>>("1 2\n1 4")), 
		(JutchsON::ParseResult<std::unordered_map<int, int>>::makeError({0, 0}, "Duplicated key detected")));
}

namespace {
	struct TestStruct {
		int ab;
		int c = 1;
		int de;

		friend bool operator == (TestStruct, TestStruct) = default;

		friend std::ostream& operator << (std::ostream& os, TestStruct value) {
			return os << "ab " << value.ab << "c " << value.c << "de " << value.de;
		}
	};

	BOOST_DESCRIBE_STRUCT(TestStruct, (), (ab, c, de));

	struct EmptyTestStruct {};

	BOOST_DESCRIBE_STRUCT(EmptyTestStruct, (), ());
}

TEST(Parse, parseStruct) {
	EXPECT_EQ(JutchsON::parse<TestStruct>("ab 1\nc 2\nde 3"), (TestStruct{1, 2, 3}));
}

TEST(Parse, parseStructDefaults) {
	EXPECT_EQ(JutchsON::parse<TestStruct>(""), (TestStruct{0, 1, 0}));
}

TEST(Parse, parseStructSomeDefaults) {
	EXPECT_EQ(JutchsON::parse<TestStruct>("ab 1"), (TestStruct{1, 1, 0}));
}

TEST(Parse, parseStructErrors) {
	JutchsON::ParseResult<TestStruct> result{std::vector<JutchsON::ParseError>{
		{{1, 2}, "'G' is not a hex digit"}, {{3, 0}, "Duplicated field de"}, {{4, 0}, "Unknown field unknown"}
	}};

	EXPECT_EQ(JutchsON::parse<TestStruct>("ab 1\n\\xGG 2\nde 3\nde 3\nunknown 4"), result);
}

TEST(Parse, parseStructEmpty) {
	EXPECT_TRUE(JutchsON::parse<EmptyTestStruct>(""));
}

TEST(Parse, parseTuple) {
	EXPECT_EQ((JutchsON::parse<std::tuple<int, int>>("1 2")), (std::tuple{1, 2}));
}

TEST(Parse, parseTupleHeterogeneous) {
	EXPECT_EQ((JutchsON::parse<std::tuple<int, std::string>>("1 abc")), (std::tuple<int, std::string>{1, "abc"}));
}

TEST(Parse, parseTupleWrongSize) {
	EXPECT_EQ((JutchsON::parse<std::tuple<int, int>>("1 2 3")), 
		(JutchsON::ParseResult<std::tuple<int, int>>::makeError({0, 0}, "Tuple size should be 2 but it is 3")));
}

TEST(Parse, parseTupleSingleton) {
	EXPECT_EQ(JutchsON::parse<std::tuple<int>>("1"), std::tuple{1});
}

TEST(Parse, parseTupleEmpty) {
	EXPECT_TRUE(JutchsON::parse<std::tuple<>>(""));
}

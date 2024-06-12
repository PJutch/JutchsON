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

#include "JutchsON.hpp"

#include <gtest/gtest.h>

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

TEST(Struct, parseStruct) {
	EXPECT_EQ(JutchsON::parse<TestStruct>("ab 1\nc 2\nde 3"), (TestStruct{1, 2, 3}));
}

TEST(Struct, parseStructDefaults) {
	EXPECT_EQ(JutchsON::parse<TestStruct>(""), (TestStruct{0, 1, 0}));
}

TEST(Struct, parseStructSomeDefaults) {
	EXPECT_EQ(JutchsON::parse<TestStruct>("ab 1"), (TestStruct{1, 1, 0}));
}

TEST(Struct, parseStructErrors) {
	JutchsON::ParseResult<TestStruct> result{std::vector<JutchsON::ParseError>{
		{{1, 2}, "'G' is not a hex digit"}, {{3, 0}, "Duplicated field de"}, {{4, 0}, "Unknown field unknown"}
	}};

	EXPECT_EQ(JutchsON::parse<TestStruct>("ab 1\n\\xGG 2\nde 3\nde 3\nunknown 4"), result);
}

TEST(Struct, parseStructDirectory) {
	EXPECT_EQ(JutchsON::parseFile<TestStruct>("../../../../tests/parse/files/dir"), (TestStruct{123, 456, 789}));
}

TEST(Struct, parseStructEmpty) {
	EXPECT_TRUE(JutchsON::parse<EmptyTestStruct>(""));
}

TEST(Struct, parseStructEmptyDirectory) {
	EXPECT_TRUE(JutchsON::parseFile<EmptyTestStruct>("../../../../tests/parse/files/emptyDir"));
}

namespace {
	struct TestType {};
	struct TestEnv {};

	struct TestTypeStruct {
		TestType a;
		TestType b;
		TestType c;
	};

	BOOST_DESCRIBE_STRUCT(TestTypeStruct, (), (a, b, c))
}

namespace JutchsON {
	template <>
	struct Parser<TestType> {
		ParseResult<TestType> operator() (StringView, TestEnv, Context) {
			return {{}};
		}
	};
}

TEST(Struct, parseStructEnv) {
	EXPECT_TRUE(JutchsON::parse<TestTypeStruct>("a 1\nb 2\nc 3", TestEnv{}));
}

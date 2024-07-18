#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Tuple, parseTuple) {
    EXPECT_EQ((JutchsON::parse<std::tuple<int, int>>("1 2")), (std::tuple{1, 2}));
}

TEST(Tuple, parseTupleHeterogeneous) {
    EXPECT_EQ((JutchsON::parse<std::tuple<int, std::string>>("1 abc")), (std::tuple<int, std::string>{1, "abc"}));
}

TEST(Tuple, parseTupleWrongSize) {
    EXPECT_EQ((JutchsON::parse<std::tuple<int, int>>("1 2 3")),
        (JutchsON::ParseResult<std::tuple<int, int>>::makeError({0, 0}, "Tuple size should be 2 but it is 3")));
}

TEST(Tuple, parseTupleSingleton) {
    EXPECT_EQ(JutchsON::parse<std::tuple<int>>("1"), std::tuple{1});
}

TEST(Tuple, parseTupleEmpty) {
    EXPECT_TRUE(JutchsON::parse<std::tuple<>>(""));
}

namespace {
    struct TestTuple {
        int a;
        int b;

        friend bool operator == (TestTuple, TestTuple) = default;
    };

    template <size_t i>
    int get(const TestTuple& t) {
        if constexpr (i == 0) {
            return t.a;
        } else {
            return t.b;
        }
    }

    template <size_t i>
    int& get(TestTuple& t) {
        if constexpr (i == 0) {
            return t.a;
        } else {
            return t.b;
        }
    }
}

namespace std {
    template <>
    struct tuple_size<TestTuple> : std::integral_constant<size_t, 2> {};

    template <size_t i>
    struct tuple_element<i, TestTuple>  {
        using type = int;
    };
}

TEST(Tuple, parseCustomTuple) {
    EXPECT_EQ(JutchsON::parse<TestTuple>("1 2"), (TestTuple{1, 2}));
}

namespace {
    struct TestType {};
    struct TestEnv {};
}

namespace JutchsON {
    template <>
    struct Parser<TestType> {
        ParseResult<TestType> operator() (StringView, TestEnv, Context) {
            return {{}};
        }
    };
}

TEST(Tuple, parseTupleEnv) {
    EXPECT_TRUE((JutchsON::parse<std::tuple<TestType, TestType>>("a b", TestEnv{})));
}

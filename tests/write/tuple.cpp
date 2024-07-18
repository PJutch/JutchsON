#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Tuple, writeTupleInt) {
    EXPECT_EQ(JutchsON::write(std::tuple{1, 2, 3}), "1 2 3");
}

TEST(Tuple, writeTupleHeterogeneous) {
    EXPECT_EQ(JutchsON::write(std::tuple<int, std::string>{1, "abcdefg"}), "1 abcdefg");
}

TEST(Tuple, writeTupleSingleton) {
    EXPECT_EQ(JutchsON::write(std::tuple<int>{1}), "1");
}

TEST(Tuple, writeTupleEmpty) {
    EXPECT_EQ(JutchsON::write(std::tuple<>{}), "");
}

TEST(Tuple, writeTupleNested) {
    EXPECT_EQ(JutchsON::write(std::tuple<int, std::tuple<int, int>>{1, {2, 3}}), "[\n    1\n    2 3\n]");
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
    struct tuple_element<i, TestTuple> {
        using type = int;
    };
}

TEST(Tuple, writeCustomTuple) {
    EXPECT_EQ(JutchsON::write(TestTuple{1, 2}), "1 2");
}

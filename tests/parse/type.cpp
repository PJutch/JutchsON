#include "JutchsON.hpp"

#include <gtest/gtest.h>

namespace {
    struct TestType1 {};
    struct TestType2 {};
}

TEST(Type, parseType) {
    std::tuple typenames{
        JUTCHSON_TAGGED_TYPE_NAME(TestType1),
        JUTCHSON_TAGGED_TYPE_NAME(TestType2)
    };

    EXPECT_TRUE(*parseType("TestType1", typenames, [](auto tag) {
        return std::same_as<JutchsON::PayloadType<decltype(tag)>, TestType1>;
    }));
}

TEST(Type, parseTypeSecond) {
    std::tuple typenames{
        JUTCHSON_TAGGED_TYPE_NAME(TestType1),
        JUTCHSON_TAGGED_TYPE_NAME(TestType2)
    };

    EXPECT_TRUE(*parseType("TestType2", typenames, [](auto tag) {
        return std::same_as<JutchsON::PayloadType<decltype(tag)>, TestType2>;
    }));
}

TEST(Type, parseTypeUnknown) {
    std::tuple typenames{
        JUTCHSON_TAGGED_TYPE_NAME(TestType1),
        JUTCHSON_TAGGED_TYPE_NAME(TestType2)
    };

    EXPECT_EQ(parseType("garbage", typenames, [](auto tag) {
        return std::same_as<JutchsON::PayloadType<decltype(tag)>, TestType1>;
    }), JutchsON::ParseResult<bool>::makeError({0, 0}, "Unknown type garbage"));
}

#include "JutchsON.hpp"

#include <gtest/gtest.h>

struct TestTypeBase {
    virtual bool test1(int) const {
        return false;
    }

    virtual bool test2(std::string_view) const {
        return false;
    }
};

struct TestType1 : public TestTypeBase {
    int a;

    bool test1(int a_) const final {
        return a == a_;
    }
};

struct TestType2 : public TestTypeBase {
    std::string b;

    bool test2(std::string_view b_) const final {
        return b == b_;
    }
};

BOOST_DESCRIBE_STRUCT(TestType1, (), (a))
BOOST_DESCRIBE_STRUCT(TestType2, (), (b))

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

TEST(Type, parseTypeVariant1) {
    std::tuple typenames{
        JUTCHSON_TAGGED_TYPE_NAME(TestType1),
        JUTCHSON_TAGGED_TYPE_NAME(TestType2)
    };

    EXPECT_TRUE((*parseTypeVariant<TestTypeBase>("TestType1 a 123", typenames))->test1(123));
}

TEST(Type, parseTypeVariant2) {
    std::tuple typenames{
        JUTCHSON_TAGGED_TYPE_NAME(TestType1),
        JUTCHSON_TAGGED_TYPE_NAME(TestType2)
    };

    EXPECT_TRUE((*parseTypeVariant<TestTypeBase>("TestType2 b abc", typenames))->test2("abc"));
}

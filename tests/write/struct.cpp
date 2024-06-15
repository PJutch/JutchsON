#include "JutchsON.hpp"

#include <gtest/gtest.h>

namespace {
    struct TestStruct {
        int ab;
        int c = 1;
        int de;
    };

    BOOST_DESCRIBE_STRUCT(TestStruct, (), (ab, c, de));

    struct EmptyTestStruct {};

    BOOST_DESCRIBE_STRUCT(EmptyTestStruct, (), ());
}

TEST(Struct, writeStruct) {
    EXPECT_EQ(JutchsON::write(TestStruct{1, 2, 3}), "{\n    ab 1\n    c 2\n    de 3\n}");
}

TEST(Struct, writeStructDefaults) {
    EXPECT_EQ(JutchsON::write(TestStruct{1}), "{\n    ab 1\n}");
}

TEST(Struct, writeStructEmpty) {
    EXPECT_EQ(JutchsON::write(EmptyTestStruct{}), "{}");
}

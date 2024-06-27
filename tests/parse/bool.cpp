#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Bool, parseBoolTrue) {
    EXPECT_TRUE(*JutchsON::parseBool("true"));
}

TEST(Bool, parseBoolTrueCamel) {
    EXPECT_TRUE(*JutchsON::parseBool("True"));
}

TEST(Bool, parseBoolTrueCaps) {
    EXPECT_TRUE(*JutchsON::parseBool("TRUE"));
}

TEST(Bool, parseBoolFalse) {
    EXPECT_FALSE(*JutchsON::parseBool("false"));
}

TEST(Bool, parseBoolFalseCamel) {
    EXPECT_FALSE(*JutchsON::parseBool("False"));
}

TEST(Bool, parseBoolFalseCaps) {
    EXPECT_FALSE(*JutchsON::parseBool("FALSE"));
}

TEST(Bool, parseBoolEmpty) {
    EXPECT_EQ(JutchsON::parseBool(""), JutchsON::ParseResult<bool>::makeError({0, 0}, "Empty bool isn't allowed"));
}

TEST(Bool, parseBoolEmptyAllowed) {
    EXPECT_TRUE(*JutchsON::parseBool("", true));
}

TEST(Bool, parseBoolInvalid) {
    EXPECT_EQ(JutchsON::parseBool("invalid"), JutchsON::ParseResult<bool>::makeError({0, 0}, "\"invalid\" is invalid bool value"));
}

TEST(Bool, parseGenericBool) {
    EXPECT_TRUE(*JutchsON::parse<bool>("true"));
}

TEST(Bool, parseGenericBoolEmpty) {
    EXPECT_EQ(JutchsON::parse<bool>("", {}, JutchsON::Context::LINE),
        JutchsON::ParseResult<bool>::makeError({0, 0}, "Empty bool isn't allowed"));
}

TEST(Bool, parseGenericBoolEmptyAllowed) {
    EXPECT_TRUE(*JutchsON::parse<bool>("", {}, JutchsON::Context::LINE_REST));
}

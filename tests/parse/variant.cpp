#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Variant, parseVariant) {
    EXPECT_EQ(JutchsON::parseVariant("abc de"), (std::pair<JutchsON::StringView, JutchsON::StringView>{"abc", "de"}));
}

TEST(Variant, parseVariantSpace) {
    EXPECT_EQ(JutchsON::parseVariant("abc de gj"), (std::pair<JutchsON::StringView, JutchsON::StringView>{"abc", "de gj"}));
}

TEST(Variant, parseVariantNoValue) {
    EXPECT_EQ(JutchsON::parseVariant("abc"), (std::pair<JutchsON::StringView, JutchsON::StringView>{"abc", ""}));
}

TEST(Variant, parseVariantEmpty) {
    EXPECT_EQ(JutchsON::parseVariant(""), 
        (JutchsON::ParseResult<std::pair<JutchsON::StringView, JutchsON::StringView>>::makeError({0, 0}, "Variant can't be empty")));
}

TEST(Variant, parseStdVariantFirst) {
    EXPECT_EQ((JutchsON::parse<std::variant<int, std::string>>("0 123")), (std::variant<int, std::string>{123}));
}

TEST(Variant, parseStdVariantSecond) {
    EXPECT_EQ((JutchsON::parse<std::variant<int, std::string>>("1 abc def j")), (std::variant<int, std::string>{"abc def j"}));
}

TEST(Variant, parseStdVariantTypeNotAnInt) {
    EXPECT_EQ((JutchsON::parse<std::variant<int, std::string>>("garbage 123")), 
        (JutchsON::ParseResult<std::variant<int, std::string>>::makeError({0, 0}, "Number can't contain char 'g'")));
}

TEST(Variant, parseStdVariantTypeOutOfRange) {
    EXPECT_EQ((JutchsON::parse<std::variant<int, std::string>>("2 123")),
        (JutchsON::ParseResult<std::variant<int, std::string>>::makeError({0, 0}, "Holded value type should be less then 2")));
}

TEST(Variant, parseStdVariantInvalidValue) {
    EXPECT_EQ((JutchsON::parse<std::variant<int, std::string>>("0 garbage")),
        (JutchsON::ParseResult<std::variant<int, std::string>>::makeError({0, 2}, "Number can't contain char 'g'")));
}

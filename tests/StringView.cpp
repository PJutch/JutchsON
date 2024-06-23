#include "JutchsON.hpp"

#include <algorithm>
#include <ranges>

#include <gtest/gtest.h>

static_assert(std::ranges::contiguous_range<JutchsON::StringView>);

TEST(StringView, iterator) {
    EXPECT_TRUE(std::ranges::equal(JutchsON::StringView{"abc"}, std::string_view{"abc"}));
}

TEST(StringView, reverse_iterator) {
    JutchsON::StringView s{"abc"};
    std::string_view result{"cba"};
    EXPECT_TRUE(std::ranges::equal(s.rbegin(), s.rend(), result.begin(), result.end()));
}

TEST(StringView, substr) {
    EXPECT_TRUE(std::ranges::equal(JutchsON::StringView{"abcdefg"}.substr(2, 3), std::string_view{"cde"}));
}

TEST(StringView, substrTooBig) {
    EXPECT_TRUE(std::ranges::equal(JutchsON::StringView{"abcdefg"}.substr(2, 100), std::string_view{"cdefg"}));
}

TEST(StringView, substrNoCount) {
    EXPECT_TRUE(std::ranges::equal(JutchsON::StringView{"abcdefg"}.substr(2), std::string_view{"cdefg"}));
}

TEST(StringView, substrSubstr) {
    EXPECT_TRUE(std::ranges::equal(JutchsON::StringView{"abcdefg"}.substr(2, 5).substr(0, 3), std::string_view{"cde"}));
}

TEST(StringView, remove_prefix) {
    auto s = JutchsON::StringView{"abcdefg"};
    s.remove_prefix(2);
    EXPECT_TRUE(std::ranges::equal(s, std::string_view{"cdefg"}));
}

TEST(StringView, remove_suffix) {
    auto s = JutchsON::StringView{"abcdefg"};
    s.remove_suffix(2);
    EXPECT_TRUE(std::ranges::equal(s, std::string_view{"abcde"}));
}

TEST(StringView, location) {
    EXPECT_EQ(JutchsON::StringView{"a\nbcdefg"}.location(7), (JutchsON::Location{1, 5}));
}

TEST(StringView, locationSubstr) {
    EXPECT_EQ(JutchsON::StringView{"a\nbcdefg"}.substr(2, 3).location(2), (JutchsON::Location{1, 2}));
}

TEST(StringView, locationDefaultSubstr) {
    EXPECT_EQ(JutchsON::StringView{"a\nbcdefg"}.substr(2).location(), (JutchsON::Location{1, 0}));
}

TEST(StringView, locationFromIters) {
    JutchsON::StringView s{"a\nbcdefg"};
    EXPECT_EQ((JutchsON::StringView{s.begin() + 2, s.begin() + 5}.location(2)), (JutchsON::Location{1, 2}));
}

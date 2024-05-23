#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Location, combine) {
    EXPECT_EQ(combine(JutchsON::Location{3, 15}, JutchsON::Location{2, 10}), (JutchsON::Location{5, 10}));
}

TEST(Location, combineSameLine) {
    EXPECT_EQ(combine(JutchsON::Location{3, 15}, JutchsON::Location{0, 10}), (JutchsON::Location{3, 25}));
}

TEST(Location, fromIndex) {
    EXPECT_EQ(JutchsON::Location::fromIndex("ae\nb\nbc\n0123", 10), (JutchsON::Location{3, 2}));
}

TEST(Location, fromIndexStart) {
    EXPECT_EQ(JutchsON::Location::fromIndex("ae\nb\nbc\n0123", 8), (JutchsON::Location{3, 0}));
}

TEST(Location, fromIndexEndLine) {
    EXPECT_EQ(JutchsON::Location::fromIndex("ae\nb\nbc\n0123", 7), (JutchsON::Location{2, 2}));
}

TEST(Location, fromIndexLine1) {
    EXPECT_EQ(JutchsON::Location::fromIndex("ae\nb\nbc\n0123", 1), (JutchsON::Location{0, 1}));
}

TEST(Location, fromIndexLine1Start) {
    EXPECT_EQ(JutchsON::Location::fromIndex("ae\nb\nbc\n0123", 0), (JutchsON::Location{0, 0}));
}

TEST(Location, fromIndexEndFile) {
    EXPECT_EQ(JutchsON::Location::fromIndex("ae\nb\nbc\n0123", 12), (JutchsON::Location{3, 4}));
}

#include "JutchsON.hpp"

#include <gtest/gtest.h>

TEST(Dict, writeDict) {
    std::vector<std::pair<std::string, std::string>> data{{"abc", "def xyz"}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::writeDict(data), "{\n    abc def xyz\n    gh ij\n}");
}

TEST(Dict, writeDictEmptyValue) {
    std::vector<std::pair<std::string, std::string>> data{{"abc", ""}, {"gh", "ij"}};
    EXPECT_EQ(JutchsON::writeDict(data), "{\n    abc\n    gh ij\n}");
}

TEST(Dict, writeDictSingleton) {
    std::vector<std::pair<std::string, std::string>> data{{"abc", "def xyz"}};
    EXPECT_EQ(JutchsON::writeDict(data), "{\n    abc def xyz\n}");
}

TEST(Dict, writeDictEmpty) {
    std::vector<std::pair<std::string, std::string>> data{};
    EXPECT_EQ(JutchsON::writeDict(data), "{}");
}

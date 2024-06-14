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

TEST(Dict, writeUnorderedMapIntInt) {
    EXPECT_EQ(JutchsON::write(std::unordered_map<int, int>{{1, 2}, {3, 4}}), "{\n    1 2\n    3 4\n}");
}

TEST(Dict, writeUnorderedMapEmpty) {
    EXPECT_EQ(JutchsON::write(std::unordered_map<int, int>{}), "{}");
}

TEST(Dict, writeUnorderedMapIntString) {
    EXPECT_EQ(JutchsON::write(std::unordered_map<int, std::string>{{1, "ab c"}, {2, "de"}}),
        "{\n    1 ab c\n    2 de\n}");
}

TEST(Dict, writeUnorderedMapStringString) {
    std::unordered_map<std::string, std::string> value{{"ab c", "de fg"}, {"xyz", "w"}};
    EXPECT_EQ(JutchsON::write(value), "{\n    \"ab c\" de fg\n    xyz w\n}");
}

TEST(Dict, writeUnorderedMapNested) {
    std::unordered_map<int, std::unordered_map<int, int>> value{{1, {{1, 2}, {3, 4}}}, {2, {{5, 6}, {7, 8}}}};
    EXPECT_EQ(JutchsON::write(value), "{\n    1 {\n        1 2\n        3 4\n    }\n    2 {\n        5 6\n        7 8\n    }\n}");
}

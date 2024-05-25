#include "ParseResult.hpp"

#include <algorithm>
#include <string_view>
#include <cctype>

inline ptrdiff_t findObjectBegin(std::string_view s) {
    return std::ranges::find_if(s, [](char c) {
        return !isspace(c);
    }) - s.begin();
}

inline ptrdiff_t findObjectEnd(std::string_view s) {
    return std::ranges::find_if(s, [](char c) {
        return isspace(c);
    }) - s.begin();
}

inline std::vector<std::string_view> parseList(std::string_view s) {
    std::vector<std::string_view> res;
    ptrdiff_t begin = findObjectBegin(s);
    while (begin < std::ssize(s)) {
        ptrdiff_t end = begin + findObjectEnd(s.substr(begin));
        res.push_back(s.substr(begin, end - begin));
        begin = end + findObjectBegin(s.substr(end));
    }
    return res;
}

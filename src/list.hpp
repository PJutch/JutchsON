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
    if (s.empty()) {
        return 0;
    }

    if (s.front() == '[') {
        ptrdiff_t balance = 0;
        ptrdiff_t i = 0;
        do {
            if (s[i] == '[') {
                ++balance;
            } else if (s[i] == ']') {
                --balance;
            }
            ++i;
        } while (balance > 0 && i < std::ssize(s));
        return i;
    } else {
        return std::ranges::find_if(s, [](char c) {
            return isspace(c) || c == '[';
        }) - s.begin();
    }
}

inline JutchsON::ParseResult<std::vector<std::string_view>> parseList(std::string_view s) {
    if (s.empty()) {
        return std::vector<std::string_view>{};
    }

    if (s.front() == '[') {
        if (s.back() != ']') {
            return JutchsON::ParseResult<std::vector<std::string_view>>::makeError({0, 0}, "Unmatched [");
        }
        s.remove_prefix(1);
        s.remove_suffix(1);
    } else if (s.back() == ']') {
        return JutchsON::ParseResult<std::vector<std::string_view>>::makeError(JutchsON::Location::fromIndex(s, std::ssize(s) - 1), "Unmatched ]");
    }

    std::vector<std::string_view> res;
    ptrdiff_t begin = findObjectBegin(s);
    while (begin < std::ssize(s)) {
        ptrdiff_t end = begin + findObjectEnd(s.substr(begin));
        res.push_back(s.substr(begin, end - begin));
        begin = end + findObjectBegin(s.substr(end));
    }
    return res;
}

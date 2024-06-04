#ifndef JUTCHSON_OBJECT_HPP_
#define JUTCHSON_OBJECT_HPP_

#include "StringView.hpp"

#include <vector>

namespace JutchsON {
    inline ptrdiff_t findObjectBegin(StringView s) {
        return std::ranges::find_if(s, [](char c) {
            return !isspace(c);
        }) - s.begin();
    }

    inline ptrdiff_t findObjectEnd(StringView s) {
        if (s.empty()) {
            return 0;
        }

        if (s.front() == '[' || s.front() == '{') {
            std::vector<char> brackets;
            ptrdiff_t i = 0;
            do {
                if (s[i] == '[' || s[i] == '{') {
                    brackets.push_back(s[i]);
                } else if (s[i] == ']') {
                    if (brackets.back() != '[') {
                        return i;
                    }
                    brackets.pop_back();
                } else if (s[i] == '}') {
                    if (brackets.back() != '{') {
                        return i;
                    }
                    brackets.pop_back();
                }
                ++i;
            } while (std::ssize(brackets) > 0 && i < std::ssize(s));
            return i;
        } else {
            return std::ranges::find_if(s, [](char c) {
                return isspace(c) || c == '[' || c == '{';
            }) - s.begin();
        }
    }

    inline ptrdiff_t findLineObjectBegin(StringView s) {
        return std::ranges::find_if(s, [](char c) {
            return !isspace(c);
        }) - s.begin();
    }

    inline ptrdiff_t findLineObjectEnd(StringView s) {
        auto i = s.begin();
        while (i != s.end()) {
            auto next = std::ranges::find_if(i, s.end(), [](char c) {
                return !isspace(c) || c == '\n';
            });

            if (next == s.end() || *next == '\n') {
                return next - s.begin();
            }

            i = next + findObjectEnd({next, s.end()});
        }
        return std::ssize(s);
    }
}

#endif
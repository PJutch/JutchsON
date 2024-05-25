#ifndef JUTCHSON_LIST_HPP_
#define JUTCHSON_LIST_HPP_

#include "ParseResult.hpp"

#include <algorithm>
#include <string_view>
#include <cctype>

namespace JutchsON {
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

    inline ptrdiff_t findLineObjectBegin(std::string_view s) {
        return std::ranges::find_if(s, [](char c) {
            return !isspace(c);
        }) - s.begin();
    }

    inline ptrdiff_t findLineObjectEnd(std::string_view s) {
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

    inline bool isMultiline(std::string_view s) {
        ptrdiff_t begin = findObjectBegin(s);
        while (begin < std::ssize(s)) {
            ptrdiff_t end = begin + findObjectEnd(s.substr(begin));
            ptrdiff_t next = end + findObjectBegin(s.substr(end));

            if (next != std::ssize(s) && std::ranges::find(s.begin() + end, s.begin() + next, '\n') != s.begin() + next) {
                return true;
            }

            begin = next;
        }
        return false;
    }

    inline ParseResult<std::vector<std::string_view>> parseList(std::string_view s) {
        if (s.empty()) {
            return std::vector<std::string_view>{};
        }

        if (s.front() == '[') {
            if (s.back() != ']') {
                return ParseResult<std::vector<std::string_view>>::makeError({0, 0}, "Unmatched [");
            }
            s.remove_prefix(1);
            s.remove_suffix(1);
        } else if (s.back() == ']') {
            return ParseResult<std::vector<std::string_view>>::makeError(Location::fromIndex(s, std::ssize(s) - 1), "Unmatched ]");
        }

        std::vector<std::string_view> res;
        if (isMultiline(s)) {
            ptrdiff_t begin = findLineObjectBegin(s);
            while (begin < std::ssize(s)) {
                ptrdiff_t end = begin + findLineObjectEnd(s.substr(begin));
                res.push_back(s.substr(begin, end - begin));
                begin = end + findLineObjectBegin(s.substr(end));
            }
        } else {
            ptrdiff_t begin = findObjectBegin(s);
            while (begin < std::ssize(s)) {
                ptrdiff_t end = begin + findObjectEnd(s.substr(begin));
                res.push_back(s.substr(begin, end - begin));
                begin = end + findObjectBegin(s.substr(end));
            }
        }
        return res;
    }
}

#endif
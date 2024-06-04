#ifndef JUTCHSON_OBJECT_HPP_
#define JUTCHSON_OBJECT_HPP_

#include "ParseResult.hpp"
#include "StringView.hpp"

#include <vector>

namespace JutchsON {
    inline ptrdiff_t findOnelineObjectBegin(StringView s) {
        return std::ranges::find_if(s, [](char c) {
            return !isspace(c);
        }) - s.begin();
    }

    inline ParseResult<ptrdiff_t> findOnelineObjectEnd(StringView s) {
        if (s.empty()) {
            return 0;
        }

        if (s.front() == '[' || s.front() == '{') {
            std::vector<char> brackets;
            for (ptrdiff_t i = 0; i < std::ssize(s); ++i) {
                if (s[i] == '[' || s[i] == '{') {
                    brackets.push_back(s[i]);
                } else if (s[i] == ']') {
                    if (brackets.back() == '{') {
                        return ParseResult<ptrdiff_t>::makeError(s.location(i), "Expected }, got ]");
                    }
                    brackets.pop_back();
                } else if (s[i] == '}') {
                    if (brackets.back() == '[') {
                        return ParseResult<ptrdiff_t>::makeError(s.location(i), "Expected ], got }");
                    }
                    brackets.pop_back();
                }
                
                if (brackets.empty()) {
                    return i + 1;
                }
            };

            if (brackets.back() == '[') {
                return ParseResult<ptrdiff_t>::makeError(s.location(std::ssize(s)), "Expected ], got eof");
            } else {
                return ParseResult<ptrdiff_t>::makeError(s.location(std::ssize(s)), "Expected }, got eof");
            }
        } else {
            return std::ranges::find_if(s, [](char c) {
                return isspace(c) || c == '[' || c == '{';
            }) - s.begin();
        }
    }

    inline ptrdiff_t findMultilineObjectBegin(StringView s) {
        return std::ranges::find_if(s, [](char c) {
            return !isspace(c);
        }) - s.begin();
    }

    inline ParseResult<ptrdiff_t> findMultilineObjectEnd(StringView s) {
        auto i = s.begin();
        while (i != s.end()) {
            auto next = std::ranges::find_if(i, s.end(), [](char c) {
                return !isspace(c) || c == '\n';
            });

            if (next == s.end() || *next == '\n') {
                return next - s.begin();
            }

            if (auto objectEnd = findOnelineObjectEnd({next, s.end()})) {
                i = next + *objectEnd;
            }
        }
        return std::ssize(s);
    }

    inline ptrdiff_t findObjectBegin(StringView s, bool isMultiline) {
        if (isMultiline) {
            return findMultilineObjectBegin(s);
        } else {
            return findOnelineObjectBegin(s);
        }
    }

    inline ParseResult<ptrdiff_t> findObjectEnd(StringView s, bool isMultiline) {
        if (isMultiline) {
            return findMultilineObjectEnd(s);
        } else {
            return findOnelineObjectEnd(s);
        }
    }

    inline ParseResult<bool> isMultiline(StringView s) {
        ptrdiff_t begin = findOnelineObjectBegin(s);
        while (begin < std::ssize(s)) {
            if (auto objectEnd = findOnelineObjectEnd(s.substr(begin))) {
                ptrdiff_t end = begin + *objectEnd;
                ptrdiff_t next = end + findOnelineObjectBegin(s.substr(end));
                if (next != std::ssize(s) && std::ranges::find(s.begin() + end, s.begin() + next, '\n') != s.begin() + next) {
                    return true;
                }
                begin = next;
            } else {
                return objectEnd.errors();
            }
        }
        return false;
    }
}

#endif
#ifndef JUTCHSON_OBJECT_HPP_
#define JUTCHSON_OBJECT_HPP_

#include "ParseResult.hpp"
#include "StringView.hpp"

#include <vector>

namespace JutchsON {
    inline ptrdiff_t findOnelineObjectBegin(StringView s) {
        return std::ranges::find_if(s, [](char c) {
            return !isspace(c) || c == '\n';
        }) - s.begin();
    }

    inline ParseResult<ptrdiff_t> findOnelineObjectEnd(StringView s) {
        if (s.empty()) {
            return 0;
        }

        if (s.front() == '[' || s.front() == '{' || s.front() == '"') {
            std::vector<char> brackets;
            for (ptrdiff_t i = 0; i < std::ssize(s); ++i) {
                if (!brackets.empty() && brackets.back() == '\\') {
                    brackets.pop_back();
                } else if (!brackets.empty() && brackets.back() == '"') {
                    if (s[i] == '"') {
                        brackets.pop_back();
                    } else if (s[i] == '\\') {
                        brackets.push_back('\\');
                    }
                } else if (s[i] == '[' || s[i] == '{' || s[i] == '"') {
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
            } else if (brackets.back() == '{') {
                return ParseResult<ptrdiff_t>::makeError(s.location(std::ssize(s)), "Expected }, got eof");
            } else if (brackets.back() == '"') {
                return ParseResult<ptrdiff_t>::makeError(s.location(std::ssize(s)), "Expected closing \", got eof");
            } else {
                return ParseResult<ptrdiff_t>::makeError(s.location(std::ssize(s)), "Expected escape sequence, got eof after \\");
            }
        } else if (s.front() == ']' || s.front() == '}') {
            return ParseResult<ptrdiff_t>::makeError(s.location(), std::format("Unexpected {}", s.front()));
        } else {
            return std::ranges::find_if(s, [](char c) {
                return isspace(c) || c == '[' || c == '{' || c == '"' || c == ']' || c == '{';
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
                return i - s.begin();
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

    inline bool isObjectBegin(StringView s, ptrdiff_t i) {
        return 0 <= i && i < std::ssize(s) && !isspace(s[i]);
    }

    inline ParseResult<bool> isMultiline(StringView s) {
        s = strip(s);

        ptrdiff_t begin = 0;
        while (begin < std::ssize(s)) {
            if (auto objectEnd = findOnelineObjectEnd(s.substr(begin))) {
                ptrdiff_t end = begin + *objectEnd;

                ptrdiff_t next = end + findOnelineObjectBegin(s.substr(end));
                if (next < std::ssize(s) && !isObjectBegin(s, next)) {
                    return true;
                }
                begin = next;
            } else {
                return objectEnd.errors();
            }
        }
        return false;
    }

    inline std::string indent(std::string_view s) {
        std::string res;

        auto begin = s.begin();
        while (true) {
            auto end = std::ranges::find(begin, s.end(), '\n');

            if (begin != end) {
                res.append(4, ' ');
                res.append(begin, end);
            }

            if (end != s.end()) {
                res.push_back('\n');
                begin = std::next(end);
            } else {
                return res;
            }
        }
    }
}

#endif
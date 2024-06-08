#ifndef JUTCHSON_STR_HPP_
#define JUTCHSON_STR_HPP_

#include "ParseResult.hpp"
#include "StringView.hpp"
#include "object.hpp"
#include "escape.hpp"

#include <format>
#include <string>
#include <algorithm>

namespace JutchsON {
    inline ParseResult<ptrdiff_t> parseHexDigit(StringView s, ptrdiff_t i) {
        if ('0' <= s[i] && s[i] <= '9') {
            return s[i] - '0';
        } else if ('A' <= s[i] && s[i] <= 'F') {
            return (s[i] - 'A') + 10;
        } else if ('a' <= s[i] && s[i] <= 'f') {
            return (s[i] - 'a') + 10;
        } else {
            return ParseResult<ptrdiff_t>::makeError(s.location(i), std::format("{} is not a hex digit", escapeChar(s, i)));
        }
    }

    inline ParseResult<std::string> parseStr(StringView s) {
        if (s.empty()) {
            return std::string{};
        }

        bool quoted = false;
        if (s.front() == '"') {
            if (s.back() != '"') {
                return ParseResult<std::string>::makeError(s.location(), "Unmatched \"");
            }

            quoted = true;
            s.remove_prefix(1);
            s.remove_suffix(1);
        } else if (s.back() == '"') {
            return ParseResult<std::string>::makeError(s.location(std::ssize(s) - 1), "Unmatched \"");
        }

        std::string res;
        res.reserve(std::size(s));
        for (int64_t i = 0; i < std::ssize(s); ++i) {
            if (s[i] == '\\') {
                if (i + 1 >= std::ssize(s)) {
                    if (quoted) {
                        return ParseResult<std::string>::makeError(s.location(-1), "Unmatched \"");
                    } else {
                        return ParseResult<std::string>::makeError(s.location(std::ssize(s)), "Truncated escape sequence");
                    }
                }

                switch (s[i + 1]) {
                case '0': res.push_back('\0'); break;
                case 'a': res.push_back('\a'); break;
                case 'b': res.push_back('\b'); break;
                case 'f': res.push_back('\f'); break;
                case 'n': res.push_back('\n'); break;
                case 'r': res.push_back('\r'); break;
                case 't': res.push_back('\t'); break;
                case 'v': res.push_back('\v'); break;
                case '\\': res.push_back('\\'); break;
                case '"': res.push_back('"'); break;
                case 'x': 
                    if (i + 3 >= std::ssize(s)) {
                        if (quoted) {
                            return ParseResult<std::string>::makeError(s.location(std::ssize(s)), "'\"' is not a hex digit");
                        } else {
                            return ParseResult<std::string>::makeError(s.location(std::ssize(s)), "Truncated \\x escape sequence");
                        }
                    }

                    if (auto parseRes = parseHexDigit(s, i + 2).then([&](ptrdiff_t a) {
                        return parseHexDigit(s, i + 3).map([&](ptrdiff_t b) {
                            return static_cast<char>(static_cast<unsigned char>(a * 16 + b));
                        });
                    })) {
                        res.push_back(*parseRes);
                    } else {
                        return parseRes.errors();
                    }
                    i += 2;
                    break;
                }

                ++i;
            } else {
                res.push_back(s[i]);
            }
        }
        return res;
    }
}

#endif
#ifndef JUTCHSON_ESCAPE_HPP_
#define JUTCHSON_ESCAPE_HPP_

#include "StringView.hpp"

#include <format>
#include <string_view>
#include <string>
#include <bit>

namespace JutchsON {
    inline ptrdiff_t getCharLen(char byte) {
        int64_t ones = std::countl_one(static_cast<unsigned char>(byte));
        if (ones == 1) {
            return 0;
        } else if (ones == 0) {
            return 1;
        } else {
            return ones;
        }
    }

    inline char digitChar(ptrdiff_t digit) {
        if (0 <= digit && digit <= 9) {
            return '0' + digit;
        } else if (10 <= digit && digit <= 35) {
            return 'A' + (digit - 10);
        } else {
            return '\0';
        }
    }

    inline std::string escapeHex(char byte) {
        auto ubyte = static_cast<unsigned char>(byte);
        return std::format("\\x{}{}", digitChar(ubyte / 16), digitChar(ubyte % 16));
    }

    inline std::string escapeChar(StringView s, ptrdiff_t i) {
        if (s.empty()) {
            return "";
        }

        ptrdiff_t charLen = getCharLen(s[i]);
        switch (s[i]) {
        case '\0': return "\\0";
        case '\a': return "\\a";
        case '\b': return "\\b";
        case '\f': return "\\f";
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\t': return "\\t";
        case '\v': return "\\v";
        default:
            if (charLen > 4 || charLen == 0 || i + charLen > std::ssize(s) || charLen == 1 && s[i] < 32) {
                return escapeHex(s[i]);
            } else {
                return s.substr(i, charLen).asString();
            }
        }
    }

    inline std::string escapeStrChar(StringView s, ptrdiff_t i) {
        if (s.empty()) {
            return "";
        }

        if (s[i] == '"' || s[i] == '\\') {
            return std::format("\\{}", s[i]);
        }

        return escapeChar(s, i);
    }
}

#endif
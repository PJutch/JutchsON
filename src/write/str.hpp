#ifndef JUTCHSON_WRITE_STR_HPP_
#define JUTCHSON_WRITE_STR_HPP_

#include "ParseResult.hpp"
#include "StringView.hpp"
#include "object.hpp"
#include "escape.hpp"

#include <format>
#include <string>
#include <algorithm>

namespace JutchsON {
    inline bool shouldBeQuouted(StringView s) {
        for (ptrdiff_t i = 0; i < std::ssize(s); i += getCharLen(s[i])) {
            if (s[i] == ' ' || s[i] == '[' || s[i] == ']' || s[i] == '{' || s[i] == '}' || s[i] == '"') {
                return true;
            }
        }
        return false;
    }

    inline std::string writeStr(StringView s, bool quoted = true) {
        bool actuallyQuoted = quoted && shouldBeQuouted(s);

        std::string res;
        if (actuallyQuoted) {
            res.push_back('"');
        }

        for (ptrdiff_t i = 0; i < std::ssize(s); i += getCharLen(s[i])) {
            res.append(escapeStrChar(s, i));
        }

        if (actuallyQuoted) {
            res.push_back('"');
        }

        return res;
    }
}

#endif
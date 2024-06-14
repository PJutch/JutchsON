#ifndef JUTCHSON_WRITE_STR_HPP_
#define JUTCHSON_WRITE_STR_HPP_

#include "escape.hpp"

#include <string_view>
#include <string>

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

    template <>
    struct Writer<std::string> {
        std::string operator() (const std::string& s, Context context) {
            return writeStr(std::string_view{s}, context == Context::OBJECT);
        }
    };

    template <>
    struct Writer<std::string_view> {
        std::string operator() (std::string_view s, Context context) {
            return writeStr(s, context == Context::OBJECT);
        }
    };

    template <>
    struct Writer<const char*> {
        std::string operator() (const char* s, Context context) {
            return writeStr(std::string_view{s}, context == Context::OBJECT);
        }
    };

    template <>
    struct Writer<char*> {
        std::string operator() (char* s, Context context) {
            return writeStr(std::string_view{s}, context == Context::OBJECT);
        }
    };

    template <>
    struct Writer<StringView> {
        std::string operator() (StringView s, Context context) {
            return writeStr(s, context == Context::OBJECT);
        }
    };

    template <>
    struct forcesMultilineImpl<std::string> {
        static inline constexpr bool value = false;
    };

    template <>
    struct forcesMultilineImpl<std::string_view> {
        static inline constexpr bool value = false;
    };

    template <>
    struct forcesMultilineImpl<StringView> {
        static inline constexpr bool value = false;
    };

    template <>
    struct forcesMultilineImpl<const char*> {
        static inline constexpr bool value = false;
    };

    template <>
    struct forcesMultilineImpl<char*> {
        static inline constexpr bool value = false;
    };
}

#endif
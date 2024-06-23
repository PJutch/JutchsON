#ifndef JUTCHSON_PARSE_OPTIONAL_HPP_
#define JUTCHSON_PARSE_OPTIONAL_HPP_

#include "parse.hpp"

#include "../ParseResult.hpp"
#include "../StringView.hpp"
#include "../strip.hpp"
#include "../object.hpp"

#include <optional>

namespace JutchsON {
    inline ParseResult<std::optional<StringView>> parseOptional(StringView s) {
        s = strip(s);
        if (!s.empty() && s.front() == '<') {
            s.remove_prefix(1);
            s.remove_suffix(1);

            return std::optional{strip(s)};
        } else if (s == "null" || s == "Null" || s == "NULL") {
            return std::optional<StringView>{};
        } else {
            return std::optional{s};
        }
    }

    template <typename T>
    struct Parser<std::optional<T>> {
        ParseResult<std::optional<T>> operator() (StringView s, Context context) {
            StringView stripped = strip(s);
            bool quoted = !stripped.empty() && stripped.front() == '<';

            return parseOptional(s).then([&](std::optional<StringView> v) {
                if (v) {
                    return parse<T>(*v).map([](const T& t) {
                        return std::optional<T>{t};
                    });
                } else {
                    return ParseResult{std::optional<T>{}};
                }
            });
        }
    };
}

#endif
#ifndef JUTCHSON_PARSE_VARIANT_HPP_
#define JUTCHSON_PARSE_VARIANT_HPP_

#include "parse.hpp"

#include "../ParseResult.hpp"
#include "../StringView.hpp"
#include "../strip.hpp"
#include "../object.hpp"

#include <variant>

namespace JutchsON {
    inline ParseResult<std::pair<StringView, StringView>> parseVariant(StringView s) {
        s = strip(s);
        if (!s.empty() && s.front() == '<') {
            s.remove_prefix(1);
            s.remove_suffix(1);

            s = strip(s);
        }

        if (s.empty()) {
            return ParseResult<std::pair<StringView, StringView>>::makeError(s.location(), "Variant can't be empty");
        }

        return findOnelineObjectEnd(s).map([&](int64_t typeEnd) {
            int64_t valueBegin = typeEnd + findOnelineObjectBegin(s.substr(typeEnd));
            return std::pair<StringView, StringView>{s.substr(0, typeEnd), s.substr(valueBegin)};
        });
    }

    template <size_t i, typename Variant, typename Env>
    ParseResult<Variant> parseVariant(size_t type, Location typeLocation, StringView value, Env&& env) {
        if constexpr (i >= std::variant_size_v<Variant>) {
            return ParseResult<Variant>::makeError(typeLocation, 
                std::format("Holded value type should be less then {}", std::variant_size_v<Variant>));
        } else {
            if (type == i) {
                using ValueT = std::remove_cv_t<std::variant_alternative_t<i, Variant>>;
                return parse<ValueT>(value, std::forward<Env>(env), Context::LINE_REST).then([&](const ValueT& value) {
                    return ParseResult{Variant{std::in_place_index<i>, value}};
                });
            }
            return parseVariant<i + 1, Variant>(type, typeLocation, value, std::forward<Env>(env));
        }
    }

    template <typename... Types>
    struct Parser<std::variant<Types...>> {
        ParseResult<std::variant<Types...>> operator() (StringView s, auto&& env, Context context) {
            return parseVariant(s).then([&](auto pair) {
                auto [typeStr, valueStr] = pair;
                return parse<size_t>(typeStr).then([&](size_t type) {
                    return parseVariant<0, std::variant<Types...>>(type, typeStr.location(), valueStr, env);
                });
            });
        }
    };
}

#endif
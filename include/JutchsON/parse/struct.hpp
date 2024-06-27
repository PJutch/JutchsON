#ifndef JUTCHSON_PARSE_STRUCT_HPP_
#define JUTCHSON_PARSE_STRUCT_HPP_

#include "../struct.hpp"
#include "dict.hpp"

#include "parse.hpp"

#include "../ParseResult.hpp"
#include "../StringView.hpp"
#include "../strip.hpp"

#include <boost/describe.hpp>

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <ranges>

namespace JutchsON {
    template <typename Range, typename T, typename U>
    concept PairRange = std::ranges::input_range<Range> && requires (std::ranges::range_value_t<Range> value) {
        { std::get<0>(value) } -> std::convertible_to<T>;
        { std::get<1>(value) } -> std::convertible_to<U>;
    };

    template <Described T, PairRange<StringView, StringView> Pairs>
    ParseResult<T> makeStruct(Pairs&& pairs, auto&& env, Context valueContext) {
        std::vector<ParseError> errors;
        std::unordered_map<std::string, StringView> values;
        for (const auto& [keyStr, valueStr] : pairs) {
            StringView keyView{keyStr};
            StringView valueView{valueStr};

            auto key = parse<std::string>(keyView, env, Context::OBJECT);
            if (!key) {
                errors.reserve(std::ssize(errors) + std::ssize(key.errors()));
                for (const ParseError& error : key.errors()) {
                    errors.push_back(error);
                }
                continue;
            }

            bool found = false;
            boost::mp11::mp_for_each<boost::describe::describe_members<T, boost::describe::mod_public>>([&](auto d) {
                if (key == d.name) {
                    found = true;
                }
            });
            if (!found) {
                errors.emplace_back(keyView.location(), std::format("Unknown field {}", keyView.asStd()));
                continue;
            }

            if (!values.try_emplace(std::move(*key), valueView).second) {
                errors.emplace_back(keyView.location(), std::format("Duplicated field {}", keyView.asStd()));
            }
        }
        if (!errors.empty()) {
            return errors;
        }

        ParseResult<T> res{{}};
        boost::mp11::mp_for_each<boost::describe::describe_members<T, boost::describe::mod_public>>([&](auto d) {
            if (!values.contains(d.name)) {
                return;
            }

            using Value = std::remove_cvref_t<decltype(std::declval<T>().*d.pointer)>;
            res = res.combine(parse<Value>(values[d.name], env, valueContext), [&](const T& t, const Value& value) {
                T newT = t;
                newT.*d.pointer = value;
                return newT;
            });

        });
        return res;
    }

    template <Described T>
    struct Parser<T> {
        template <typename Env>
        ParseResult<T> operator() (StringView s, Env&& env, Context context) {
            if (context == Context::OBJECT) {
                if (auto stripped = strip(s); stripped.empty() || stripped.front() != '{') {
                    return ParseResult<T>::makeError(stripped.location(), "Expected a nested struct");
                }
            }

            return parseDict(s).then([&](const auto& pairs) {
                return isMultilineDict(s).then([&](bool multiline) {
                    return makeStruct<T>(pairs, std::forward<Env>(env), multiline ? Context::LINE_REST : Context::OBJECT);
                });
            });
        }

        template <typename Env>
        ParseResult<T> operator() (const std::filesystem::path* path, Env&& env) {
            if (!std::filesystem::is_directory(*path)) {
                return (*this)(readWholeFile(*path), std::forward<Env>(env), Context::LINE);
            }

            std::vector<std::pair<std::string, std::string>> pairs;
            for (const auto& elementPath : directoryElements(*path)) {
                pairs.emplace_back(elementPath.stem().generic_string(), readWholeFile(elementPath));
            }
            return makeStruct<T>(pairs, std::forward<Env>(env), Context::LINE);
        }
    };
}

#endif
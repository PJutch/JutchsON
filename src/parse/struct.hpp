#ifndef JUTCHSON_PARSE_STRUCT_HPP_
#define JUTCHSON_PARSE_STRUCT_HPP_

#include "../struct.hpp"
#include "parse/dict.hpp"

#include "parse.hpp"

#include "ParseResult.hpp"
#include "StringView.hpp"
#include "strip.hpp"

#include <boost/describe.hpp>

#include <unordered_map>
#include <vector>

namespace JutchsON {
    template <Described T>
    struct Parser<T> {
        ParseResult<T> operator() (StringView s, Context context) {
            if (context == Context::OBJECT) {
                if (auto stripped = strip(s); stripped.empty() || stripped.front() != '{') {
                    return ParseResult<T>::makeError(stripped.location(), "Expected a nested struct");
                }
            }

            auto pairs = parseDict(s);
            if (!pairs) {
                return pairs.errors();
            }

            std::vector<ParseError> errors;
            std::unordered_map<std::string, StringView> values;
            for (auto [keyStr, valueStr] : *pairs) {
                auto key = parse<std::string>(keyStr, Context::OBJECT);
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
                    errors.emplace_back(keyStr.location(), std::format("Unknown field {}", keyStr.asStd()));
                    continue;
                }

                if (!values.try_emplace(std::move(*key), valueStr).second) {
                    errors.emplace_back(keyStr.location(), std::format("Duplicated field {}", keyStr.asStd()));
                }
            }
            if (!errors.empty()) {
                return errors;
            }

            auto multiline = isMultilineDict(s);
            if (!multiline) {
                return multiline.errors();
            }

            ParseResult<T> res{{}};
            boost::mp11::mp_for_each<boost::describe::describe_members<T, boost::describe::mod_public>>([&](auto d) {
                if (!values.contains(d.name)) {
                    return;
                }

                using Value = std::remove_cvref_t<decltype(std::declval<T>().*d.pointer)>;
                res = res.combine(parse<Value>(values[d.name], multiline ? Context::LINE_REST : Context::OBJECT),
                    [&](const T& t, const Value& value) {
                    T newT = t;
                    newT.*d.pointer = value;
                    return newT;
                });

            });
            return res;
        }
    };
}

#endif
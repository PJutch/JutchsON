#ifndef JUTCHSON_PARSE_DICT_HPP_
#define JUTCHSON_PARSE_DICT_HPP_

#include "struct.hpp"

#include "parse.hpp"

#include "ParseResult.hpp"
#include "StringView.hpp"
#include "object.hpp"
#include "strip.hpp"

#include <boost/describe.hpp>

#include <unordered_map>
#include <vector>
#include <utility>

namespace JutchsON {
    inline ParseResult<std::vector<std::pair<StringView, StringView>>> parseDict(StringView s) {
        s = strip(s);

        if (s.empty()) {
            return std::vector<std::pair<StringView, StringView>>{};
        }

        if (s.front() == '{') {
            ParseResult<ptrdiff_t> dictEnd = findOnelineObjectEnd(s);
            if (!dictEnd) {
                return dictEnd.errors();
            }

            if (dictEnd == std::ssize(s)) {
                s.remove_prefix(1);
                s.remove_suffix(1);
                s = strip(s);
            }
        }

        std::vector<std::pair<StringView, StringView>> res;
        return isMultiline(s).then([&](bool multiline) -> ParseResult<std::vector<std::pair<StringView, StringView>>> {
            ptrdiff_t keyBegin = 0;
            while (isObjectBegin(s, keyBegin)) {
                if (auto relKeyEnd = findOnelineObjectEnd(s.substr(keyBegin))) {
                    ptrdiff_t keyEnd = keyBegin + *relKeyEnd;
                    ptrdiff_t valueBegin = keyEnd + findOnelineObjectBegin(s.substr(keyEnd));
                    if (valueBegin >= std::ssize(s)) {
                        return ParseResult<std::vector<std::pair<StringView, StringView>>>
                            ::makeError(s.location(std::ssize(s)), "No value given for key");
                    }

                    if (auto relValueEnd = findObjectEnd(s.substr(valueBegin), multiline)) {
                        ptrdiff_t valueEnd = valueBegin + *relValueEnd;
                        res.emplace_back(s.substr(keyBegin, keyEnd - keyBegin), s.substr(valueBegin, valueEnd - valueBegin));
                        keyBegin = valueEnd + findObjectBegin(s.substr(valueEnd), multiline);
                    } else {
                        return relValueEnd.errors();
                    }
                } else {
                    return relKeyEnd.errors();
                }
            }
            return res;
        });
    }

    inline ParseResult<bool> isMultilineDict(StringView s) {
        s = strip(s);

        if (s.empty()) {
            return false;
        }

        if (s.front() == '{') {
            ParseResult<ptrdiff_t> dictEnd = findOnelineObjectEnd(s);
            if (!dictEnd) {
                return dictEnd.errors();
            }

            if (dictEnd == std::ssize(s)) {
                s.remove_prefix(1);
                s.remove_suffix(1);
                s = strip(s);
            }
        }

        return isMultiline(s);
    }

    template <typename Key, typename Value>
    struct Parser<std::unordered_multimap<Key, Value>> {
        ParseResult<std::unordered_multimap<Key, Value>> operator() (StringView s, Context context) {
            if (context == Context::OBJECT) {
                if (auto stripped = strip(s); stripped.empty() || stripped.front() != '{') {
                    return ParseResult<std::unordered_multimap<Key, Value>>::makeError(stripped.location(), "Expected a nested dict");
                }
            }

            auto pairs = parseDict(s);
            if (!pairs) {
                return pairs.errors();
            }

            auto multiline = isMultilineDict(s);
            if (!multiline) {
                return multiline.errors();
            }

            ParseResult<std::unordered_multimap<Key, Value>> res{{}};
            for (auto [key, value] : *pairs) {
                auto pair = parse<Key>(key, Context::OBJECT)
                        .combine(parse<Value>(value, *multiline ? Context::LINE_REST : Context::OBJECT), 
                            [](const Key& key, const Value& value) {
                    return std::pair{key, value};
                });

                res = res.combine(pair, [](const std::unordered_multimap<Key, Value>& map, const std::pair<Key, Value>& pair) {
                    std::unordered_multimap<Key, Value> newMap = map;
                    newMap.insert(pair);
                    return newMap;
                });
            }
            return res;
        }
    };

    template <typename Key, typename Value>
    struct Parser<std::unordered_map<Key, Value>> {
        ParseResult<std::unordered_map<Key, Value>> operator() (StringView s, Context context) {
            auto multimap = parse<std::unordered_multimap<Key, Value>>(s, context);
            if (!multimap) {
                return multimap.errors();
            }

            std::unordered_map<Key, Value> res;
            std::vector<ParseError> errors;
            for (const auto& [key, value] : *multimap) {
                if (!res.try_emplace(key, value).second) {
                    errors.emplace_back(s.location(), "Duplicated key detected");
                }
            }
            return errors.empty() ? res : ParseResult<std::unordered_map<Key, Value>>{errors};
        } 
    };

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
#ifndef JUTCHSON_PARSE_DICT_HPP_
#define JUTCHSON_PARSE_DICT_HPP_

#include "ParseResult.hpp"
#include "StringView.hpp"
#include "object.hpp"
#include "strip.hpp"

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

}

#endif
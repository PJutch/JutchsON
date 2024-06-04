#ifndef JUTCHSON_DICT_HPP_
#define JUTCHSON_DICT_HPP_

#include "ParseResult.hpp"
#include "StringView.hpp"
#include "object.hpp"

#include <algorithm>
#include <utility>

namespace JutchsON {
    inline ParseResult<std::vector<std::pair<StringView, StringView>>> parseDict(StringView s) {
        if (s.empty()) {
            return std::vector<std::pair<StringView, StringView>>{};
        }

        if (s.front() == '{') {
            if (s.back() != '}') {
                return ParseResult<std::vector<std::pair<StringView, StringView>>>::makeError(s.location(), "Unmatched {");
            }
            s.remove_prefix(1);
            s.remove_suffix(1);
        } else if (s.back() == '}') {
            return ParseResult<std::vector<std::pair<StringView, StringView>>>::makeError(s.location(std::ssize(s) - 1), "Unmatched }");
        }

        std::vector<std::pair<StringView, StringView>> res;
        return isMultiline(s).then([&](bool multiline) -> ParseResult<std::vector<std::pair<StringView, StringView>>> {
            ptrdiff_t keyBegin = findObjectBegin(s);
            while (keyBegin < std::ssize(s)) {
                if (auto relKeyEnd = findObjectEnd(s.substr(keyBegin))) {
                    ptrdiff_t keyEnd = keyBegin + *relKeyEnd;
                    ptrdiff_t valueBegin = keyEnd + (multiline ? findLineObjectBegin(s.substr(keyEnd)) : findObjectBegin(s.substr(keyEnd)));
                    if (valueBegin >= std::ssize(s)) {
                        return ParseResult<std::vector<std::pair<StringView, StringView>>>
                            ::makeError(s.location(std::ssize(s)), "No value given for key");
                    }

                    if (auto relValueEnd = (multiline ? findLineObjectEnd(s.substr(valueBegin)) : findObjectEnd(s.substr(valueBegin)))) {
                        ptrdiff_t valueEnd = valueBegin + *relValueEnd;
                        res.emplace_back(s.substr(keyBegin, keyEnd - keyBegin), s.substr(valueBegin, valueEnd - valueBegin));
                        keyBegin = valueEnd + findObjectBegin(s.substr(valueEnd));
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
}

#endif
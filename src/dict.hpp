#ifndef JUTCHSON_DICT_HPP_
#define JUTCHSON_DICT_HPP_

#include "ParseResult.hpp"
#include "StringView.hpp"
#include "object.hpp"

#include <vector>
#include <algorithm>
#include <utility>

namespace JutchsON {
    inline ParseResult<std::vector<std::pair<StringView, StringView>>> parseDict(StringView s) {
        s = strip(s);

        if (s.empty()) {
            return std::vector<std::pair<StringView, StringView>>{};
        }

        if (s.front() == '{') {
            if (s.back() != '}') {
                return ParseResult<std::vector<std::pair<StringView, StringView>>>::makeError(s.location(), "Unmatched {");
            }

            s.remove_prefix(1);
            s.remove_suffix(1);
            s = strip(s);
        } else if (s.back() == '}') {
            return ParseResult<std::vector<std::pair<StringView, StringView>>>::makeError(s.location(std::ssize(s) - 1), "Unmatched }");
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
}

#endif
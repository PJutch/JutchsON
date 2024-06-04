#ifndef JUTCHSON_LIST_HPP_
#define JUTCHSON_LIST_HPP_

#include "ParseResult.hpp"
#include "object.hpp"

#include <algorithm>
#include <string_view>
#include <cctype>

namespace JutchsON {
    inline ParseResult<std::vector<StringView>> parseList(StringView s) {
        if (s.empty()) {
            return std::vector<StringView>{};
        }

        if (s.front() == '[') {
            if (s.back() != ']') {
                return ParseResult<std::vector<StringView>>::makeError(s.location(), "Unmatched [");
            }
            s.remove_prefix(1);
            s.remove_suffix(1);
        } else if (s.back() == ']') {
            return ParseResult<std::vector<StringView>>::makeError(s.location(std::ssize(s) - 1), "Unmatched ]");
        }

        std::vector<StringView> res;
        return isMultiline(s).then([&](bool is) -> ParseResult<std::vector<StringView>> {
            if (is) {
                ptrdiff_t begin = findLineObjectBegin(s);
                while (begin < std::ssize(s)) {
                    if (auto objectEnd = findLineObjectEnd(s.substr(begin))) {
                        ptrdiff_t end = begin + *objectEnd;
                        res.push_back(s.substr(begin, end - begin));
                        begin = end + findLineObjectBegin(s.substr(end));
                    } else {
                        return objectEnd.errors();
                    }
                }
            } else {
                ptrdiff_t begin = findObjectBegin(s);
                while (begin < std::ssize(s)) {
                    if (auto objectEnd = findObjectEnd(s.substr(begin))) {
                        ptrdiff_t end = begin + *objectEnd;
                        res.push_back(s.substr(begin, end - begin));
                        begin = end + findObjectBegin(s.substr(end));
                    } else {
                        return objectEnd.errors();
                    }
                }
            }
            return res;
        });
    }
}

#endif
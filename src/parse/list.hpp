#ifndef JUTCHSON_PARSE_LIST_HPP_
#define JUTCHSON_PARSE_LIST_HPP_

#include "ParseResult.hpp"
#include "StringView.hpp"
#include "object.hpp"
#include "strip.hpp"

#include <vector>

namespace JutchsON {
    inline ParseResult<std::vector<StringView>> parseList(StringView s) {
        s = strip(s);

        if (s.empty()) {
            return std::vector<StringView>{};
        }

        if (s.front() == '[') {
            if (s.back() != ']') {
                return ParseResult<std::vector<StringView>>::makeError(s.location(), "Unmatched [");
            }

            s.remove_prefix(1);
            s.remove_suffix(1);
            s = strip(s);
        } else if (s.back() == ']') {
            return ParseResult<std::vector<StringView>>::makeError(s.location(std::ssize(s) - 1), "Unmatched ]");
        }

        return isMultiline(s).then([&](bool multiline) -> ParseResult<std::vector<StringView>> {
            std::vector<StringView> res;
            ptrdiff_t begin = 0;
            while (isObjectBegin(s, begin)) {
                if (auto objectEnd = findObjectEnd(s.substr(begin), multiline)) {
                    ptrdiff_t end = begin + *objectEnd;
                    res.push_back(s.substr(begin, end - begin));
                    begin = end + findObjectBegin(s.substr(end), multiline);
                } else {
                    return objectEnd.errors();
                }
            }
            return res;
        });
    }
}

#endif
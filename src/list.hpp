#ifndef JUTCHSON_LIST_HPP_
#define JUTCHSON_LIST_HPP_

#include "ParseResult.hpp"
#include "object.hpp"

#include <span>
#include <vector>
#include <algorithm>
#include <string_view>
#include <cctype>

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

    inline std::string writeOnelineList(std::span<const std::string> l, bool quoted = false) {
        std::string res;
        if (quoted) {
            res.push_back('[');
        }

        for (std::string_view elem : l) {
            if (!res.empty() && res.back() != '[') {
                res.push_back(' ');
            }
            res.append(elem.data(), std::ssize(elem));
        }

        if (quoted) {
            res.push_back(']');
        }
        return res;
    }

    inline std::string writeMultilineList(std::span<const std::string> l) {
        std::string res;
        res.push_back('[');

        for (std::string_view elem : l) {
            res.push_back('\n');
            res.append(indent(elem));
        }

        if (res.back() != '[') {
            res.push_back('\n');
        }
        res.push_back(']');
        return res;
    }

    inline std::string writeList(std::span<const std::string> l, bool quoted = false) {
        if (hasMultiline(l)) {
            return writeMultilineList(l);
        } else {
            return writeOnelineList(l, quoted);
        }
    }
}

#endif
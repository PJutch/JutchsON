#ifndef JUTCHSON_LIST_HPP_
#define JUTCHSON_LIST_HPP_

#include "ParseResult.hpp"
#include "object.hpp"

#include <algorithm>
#include <string_view>
#include <cctype>

namespace JutchsON {
    inline bool isMultiline(StringView s) {
        ptrdiff_t begin = findObjectBegin(s);
        while (begin < std::ssize(s)) {
            ptrdiff_t end = begin + findObjectEnd(s.substr(begin));
            ptrdiff_t next = end + findObjectBegin(s.substr(end));

            if (next != std::ssize(s) && std::ranges::find(s.begin() + end, s.begin() + next, '\n') != s.begin() + next) {
                return true;
            }

            begin = next;
        }
        return false;
    }

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
        if (isMultiline(s)) {
            ptrdiff_t begin = findLineObjectBegin(s);
            while (begin < std::ssize(s)) {
                ptrdiff_t end = begin + findLineObjectEnd(s.substr(begin));
                res.push_back(s.substr(begin, end - begin));
                begin = end + findLineObjectBegin(s.substr(end));
            }
        } else {
            ptrdiff_t begin = findObjectBegin(s);
            while (begin < std::ssize(s)) {
                ptrdiff_t end = begin + findObjectEnd(s.substr(begin));
                res.push_back(s.substr(begin, end - begin));
                begin = end + findObjectBegin(s.substr(end));
            }
        }
        return res;
    }
}

#endif
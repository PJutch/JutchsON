#ifndef JUTCHSON_PARSE_LIST_HPP_
#define JUTCHSON_PARSE_LIST_HPP_

#include "parse.hpp"

#include "../ParseResult.hpp"
#include "../StringView.hpp"
#include "../object.hpp"
#include "../strip.hpp"

#include <vector>

namespace JutchsON {
    inline ParseResult<std::vector<StringView>> parseList(StringView s) {
        s = strip(s);

        if (s.empty()) {
            return std::vector<StringView>{};
        }

        if (s.front() == '[') {
            ParseResult<ptrdiff_t> listEnd = findOnelineObjectEnd(s);
            if (!listEnd) {
                return listEnd.errors();
            }

            if (listEnd == std::ssize(s)) {
                s.remove_prefix(1);
                s.remove_suffix(1);
                s = strip(s);
            }
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

    inline ParseResult<bool> isMultilineList(StringView s) {
        s = strip(s);

        if (s.empty()) {
            return false;
        }

        if (s.front() == '[') {
            ParseResult<ptrdiff_t> listEnd = findOnelineObjectEnd(s);
            if (!listEnd) {
                return listEnd.errors();
            }

            if (listEnd == std::ssize(s)) {
                s.remove_prefix(1);
                s.remove_suffix(1);
                s = strip(s);
            }
        }

        return isMultiline(s);
    }

    template <typename T>
    struct Parser<std::vector<T>> {
        ParseResult<std::vector<T>> operator() (StringView s, Context context) {
            if (context == Context::OBJECT) {
                if (auto stripped = strip(s); stripped.empty() || stripped.front() != '[') {
                    return ParseResult<std::vector<T>>::makeError(stripped.location(), "Expected a nested list");
                }
            }

            auto elements = parseList(s);
            if (!elements) {
                return elements.errors();
            }

            auto multiline = isMultilineList(s);
            if (!multiline) {
                return multiline.errors();
            }

            ParseResult<std::vector<T>> res{{}};
            for (StringView element : *elements) {
                res = res.combine(parse<T>(element, *multiline ? Context::LINE : Context::OBJECT), 
                        [](const std::vector<T>& list, const T& element) {
                    std::vector<T> newList = list;
                    newList.push_back(element);
                    return newList;
                });
            }
            return res;
        }
    };
}

#endif
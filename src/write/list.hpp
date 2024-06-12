#ifndef JUTCHSON_WRITE_LIST_HPP_
#define JUTCHSON_WRITE_LIST_HPP_

#include "object.hpp"

#include <span>
#include <algorithm>
#include <string_view>

namespace JutchsON {
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
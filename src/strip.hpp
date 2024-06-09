#ifndef JUTCHSON_STRIP_HPP_
#define JUTCHSON_STRIP_HPP_

#include "StringView.hpp"

#include <algorithm>
#include <cctype>

namespace JutchsON {
    inline StringView strip(StringView s) {
        auto begin = std::ranges::find_if(s, [](char c) {
            return !isspace(c);
        });

        auto end = std::ranges::find_if(s.rbegin(), s.rend(), [](char c) {
            return !isspace(c);
        }).base();

        if (end <= begin) {
            end = begin;
        }
        return {begin, end};
    }
}

#endif
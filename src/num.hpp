#ifndef JUTCHSON_NUM_HPP_
#define JUTCHSON_NUM_HPP_

#include "ParseResult.hpp"

#include <format>
#include <string_view>
#include <string>
#include <cctype>

namespace JutchsON {
    template <typename T = unsigned int>
    ParseResult<T> parseUint(std::string_view s) {
        if (s.empty()) {
            return ParseResult<T>::makeError({0, 0}, "Expected uint, got empty str");
        }

        T res{};
        for (ptrdiff_t i = 0; i < std::ssize(s); ++i) {
            char c = s[i];
            if (isdigit(c)) {
                res *= static_cast<T>(10);
                res += static_cast<T>(c - '0');
            } else {
                return ParseResult<T>::makeError(Location::fromIndex(s, i), std::format("Expected uint, got unknown char {}", c));
            }
        }
        return res;
    }
}

#endif
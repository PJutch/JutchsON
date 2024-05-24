#ifndef JUTCHSON_NUM_HPP_
#define JUTCHSON_NUM_HPP_

#include "ParseResult.hpp"
#include "escape.hpp"

#include <algorithm>
#include <functional>
#include <format>
#include <string_view>
#include <string>
#include <cctype>

namespace JutchsON {
    template <typename T = unsigned int>
    ParseResult<T> parseUint(std::string_view s) {
        if (s.empty()) {
            return ParseResult<T>::makeError({0, 0}, "Number can't be empty str");
        }

        T res{};
        for (ptrdiff_t i = 0; i < std::ssize(s); ++i) {
            char c = s[i];
            if (isdigit(c)) {
                res *= static_cast<T>(10);
                res += static_cast<T>(c - '0');
            } else if (c == '\'') {
                if (i == 0) {
                    return ParseResult<T>::makeError({0, 0}, "Number can't start with '");
                } else if (s[i - 1] == '\'') {
                    return ParseResult<T>::makeError(Location::fromIndex(s, i), "Number can't contain double '");
                }
            } else {
                return ParseResult<T>::makeError(Location::fromIndex(s, i), std::format("Number can't contain char {}", escapeChar(s, i)));
            }
        }

        if (s.back() == '\'') {
            return ParseResult<T>::makeError(Location::fromIndex(s, std::ssize(s) - 1), "Number can't end with '");
        }

        return res;
    }

    template <typename T = int>
    ParseResult<T> parseInt(std::string_view s) {
        if (s.empty()) {
            return ParseResult<T>::makeError({0, 0}, "Number can't be empty str");
        } else if (s.front() == '+') {
            return parseUint<T>(s.substr(1)).offseted({0, 1});
        } else if (s.front() == '-') {
            return parseUint<T>(s.substr(1)).offseted({0, 1}).map(std::negate<>{});
        } else {
            return parseUint<T>(s);
        }
    }

    template <typename T>
    T powIntExp(T base, ptrdiff_t exp) {
        if (exp == 0) {
            return T{1};
        } else if (exp % 2 == 1) {
            return powIntExp(base, exp - 1) * base;
        } else if (exp % 2 == 0) {
            T halfPower = powIntExp(base, exp / 2);
            return halfPower * halfPower;
        } else {
            return powIntExp(base, exp + 1) / base;
        }
    }

    template <typename T = double>
    ParseResult<T> parseNonnegativeFloat(std::string_view s) {
        auto dot = std::ranges::find(s, '.');
        return parseUint({s.begin(), dot}).then([&](T integral) -> ParseResult<T> {
            if (dot == s.end() || std::next(dot) == s.end()) {
                return integral;
            }

            auto fractionalBegin = std::next(dot);
            ptrdiff_t nDigits = std::ranges::count_if(fractionalBegin, s.end(), [](char c) {
                return isdigit(c);
            });
            return parseUint<T>({fractionalBegin, s.end()}).map([&](T fractional) {
                return integral + fractional * powIntExp(T{10}, -nDigits);
            });
        });
    }

    template <typename T = double>
    ParseResult<T> parseFloat(std::string_view s) {
        if (s.empty()) {
            return ParseResult<T>::makeError({0, 0}, "Number can't be empty str");
        } else if (s.front() == '+') {
            return parseNonnegativeFloat<T>(s.substr(1)).offseted({0, 1});
        } else if (s.front() == '-') {
            return parseNonnegativeFloat<T>(s.substr(1)).offseted({0, 1}).map(std::negate<>{});
        } else {
            return parseNonnegativeFloat<T>(s);
        }
    }
}

#endif
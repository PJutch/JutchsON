#ifndef JUTCHSON_NUM_HPP_
#define JUTCHSON_NUM_HPP_

#include "StringView.hpp"
#include "ParseResult.hpp"
#include "escape.hpp"
#include "strip.hpp"

#include <algorithm>
#include <functional>
#include <format>
#include <string_view>
#include <string>
#include <cctype>

namespace JutchsON {
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

    template <typename T = unsigned int>
    ParseResult<T> parseDigits(StringView s) {
        if (s.empty()) {
            return ParseResult<T>::makeError(s.location(), "Number can't be empty str");
        }

        T res{};
        for (ptrdiff_t i = 0; i < std::ssize(s); ++i) {
            char c = s[i];
            if (isdigit(c)) {
                res *= static_cast<T>(10);
                res += static_cast<T>(c - '0');
            } else if (c == '\'') {
                if (i == 0) {
                    return ParseResult<T>::makeError(s.location(), "Number can't start with '");
                } else if (s[i - 1] == '\'') {
                    return ParseResult<T>::makeError(s.location(i), "Number can't contain double '");
                }
            } else {
                return ParseResult<T>::makeError(s.location(i), std::format("Number can't contain char {}", escapeChar(s, i)));
            }
        }

        if (s.back() == '\'') {
            return ParseResult<T>::makeError(s.location(std::ssize(s) - 1), "Number can't end with '");
        }

        return res;
    }

    template <typename T = int>
    ParseResult<T> parseSignDigits(StringView s) {
        if (s.empty()) {
            return ParseResult<T>::makeError(s.location(), "Number can't be empty str");
        } else if (s.front() == '+') {
            return parseDigits<T>(s.substr(1));
        } else if (s.front() == '-') {
            return parseDigits<T>(s.substr(1)).map(std::negate<>{});
        } else {
            return parseDigits<T>(s);
        }
    }

    template <typename T = unsigned int>
    ParseResult<T> parseUint(StringView s) {
        s = strip(s);

        auto e = std::ranges::find_if(s, [](char c) {
            return c == 'e' || c == 'E';
        });
        return parseDigits<T>({s.begin(), e}).then([&](T integral) -> ParseResult<T> {
            if (e == s.end()) {
                return integral;
            } else if (std::next(e) == s.end()) {
                return ParseResult<T>::makeError(s.location(std::next(e) - s.begin()), "Exponent must follow e");
            }

            return parseSignDigits<ptrdiff_t>({std::next(e), s.end()}).then([&](ptrdiff_t exp) -> ParseResult<T> {
                if (exp < 0) {
                    return ParseResult<T>::makeError(s.location(std::next(e) - s.begin()), "Integer exponent must be > 0");
                }
                return integral * powIntExp(T{10}, exp);
            });
        });
    }

    template <typename T = int>
    ParseResult<T> parseInt(StringView s) {
        s = strip(s);

        if (s.empty()) {
            return ParseResult<T>::makeError(s.location(), "Number can't be empty str");
        } else if (s.front() == '+') {
            return parseUint<T>(s.substr(1));
        } else if (s.front() == '-') {
            return parseUint<T>(s.substr(1)).map(std::negate<>{});
        } else {
            return parseUint<T>(s);
        }
    }

    template <typename T = double>
    ParseResult<T> parseNonnegativeFloat(StringView s) {
        s = strip(s);

        auto dot = std::ranges::find(s, '.');
        if (dot == s.end()) {
            auto e = std::ranges::find_if(s, [](char c) {
                return c == 'e' || c == 'E';
            });

            return parseDigits<T>({s.begin(), e}).then([&](T integral) -> ParseResult<T> {
                if (e == s.end()) {
                    return integral;
                } else if (std::next(e) == s.end()) {
                    return ParseResult<T>::makeError(s.location(std::next(e) - s.begin()), "Exponent must follow e");
                }

                return parseSignDigits<ptrdiff_t>({std::next(e), s.end()}).then([&](ptrdiff_t exp) -> ParseResult<T> {
                    return integral * powIntExp(T{10}, exp);
                });
            });
        }

        return parseUint(std::string_view{s.begin(), dot}).then([&](T integral) -> ParseResult<T> {
            auto e = std::ranges::find_if(std::next(dot), s.end(), [](char c) {
                return c == 'e' || c == 'E';
            });

            if (std::next(dot) == e) {
                if (e == s.end()) {
                    return integral;
                } else if (std::next(e) == s.end()) {
                    return ParseResult<T>::makeError(s.location(std::next(e) - s.begin()), "Exponent must follow e");
                }

                return parseSignDigits<ptrdiff_t>({std::next(e), s.end()}).then([&](ptrdiff_t exp) -> ParseResult<T> {
                    return integral * powIntExp(T{10}, exp);
                });
            }

            ptrdiff_t nDigits = std::ranges::count_if(std::next(dot), e, [](char c) {
                return isdigit(c);
            });

            return parseUint<T>({std::next(dot), e}).then([&](T fractional) -> ParseResult<T> {
                if (e == s.end()) {
                    return integral + fractional * powIntExp(T{10}, -nDigits);
                } else if (std::next(e) == s.end()) {
                    return ParseResult<T>::makeError(StringView{std::next(dot), s.end()}.location(std::next(e) - std::next(dot)), 
                                                                         "Exponent must follow e");
                }

                return parseSignDigits<ptrdiff_t>({std::next(e), s.end()}).map([&](ptrdiff_t exp) {
                    return integral * powIntExp(T{10}, exp) + fractional * powIntExp(T{10}, exp - nDigits);
                });
            });
        });
    }

    template <typename T = double>
    ParseResult<T> parseFloat(StringView s) {
        s = strip(s);

        if (s.empty()) {
            return ParseResult<T>::makeError(s.location(), "Number can't be empty str");
        } else if (s.front() == '+') {
            return parseNonnegativeFloat<T>(s.substr(1));
        } else if (s.front() == '-') {
            return parseNonnegativeFloat<T>(s.substr(1)).map(std::negate<>{});
        } else {
            return parseNonnegativeFloat<T>(s);
        }
    }

    template <typename T>
    std::string writeUint(T s) {
        if (s == 0) {
            return "0";
        }

        std::string res;
        while (s > 0) {
            res.push_back('0' + s % 10);
            s /= 10;
        }

        std::ranges::reverse(res);
        return res;
    }
}

#endif
#ifndef JUTCHSON_WRITE_LIST_HPP_
#define JUTCHSON_WRITE_LIST_HPP_

#include "write.hpp"

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

    template <typename T>
    struct forcesMultilineImpl {
        static inline constexpr bool value = true;
    };

    template <typename T>
    inline constexpr bool forcesMultiline = forcesMultilineImpl<T>::value;

    template <std::ranges::sized_range Range>
    bool shouldBeMultiline(Range range) {
        return forcesMultiline<std::ranges::range_value_t<Range>> || std::ssize(range) > 40;
    }

    template <std::ranges::sized_range Range> requires std::ranges::random_access_range<Range>
    bool shouldBeMultiline(Range range) {
        return forcesMultiline<std::ranges::range_value_t<Range>> || std::ssize(range) > 40;
    }

    template <std::ranges::random_access_range Range>
    bool shouldBeMultiline(Range range) {
        return forcesMultiline<std::ranges::range_value_t<Range>> || range.end() - range.begin() > 40;
    }

    template <std::ranges::range Range>
    bool shouldBeMultiline(Range range) {
        return true;
    }

    template <typename T, size_t i>
    constexpr bool anyForcesMultiline() {
        if constexpr (i >= std::tuple_size_v<T>) {
            return false;
        } else {
            return forcesMultiline<std::tuple_element_t<i, T>> || anyForcesMultiline<T, i + 1>();
        }
    }

    template <Tuplelike T>
    constexpr bool shouldBeMultiline(const T& tuple) {
        return std::tuple_size_v<T> > 40 || anyForcesMultiline<T, 0>();
    }

    template <typename T>
    struct Writer<std::vector<T>> {
        std::string operator() (const std::vector<T>& list, Context context) {
            bool multiline = shouldBeMultiline(list);
            
            std::vector<std::string> elements;
            elements.reserve(std::ssize(list));
            for (const auto& element : list) {
                elements.push_back(write(element, multiline ? Context::LINE : Context::OBJECT));
            }

            if (multiline) {
                return writeMultilineList(elements);
            } else {
                return writeOnelineList(elements, context == Context::OBJECT);
            }
        }
    };
}

#endif
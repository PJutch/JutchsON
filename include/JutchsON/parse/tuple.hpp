#ifndef JUTCHSON_PARSE_TUPLE_HPP_
#define JUTCHSON_PARSE_TUPLE_HPP_

#include "../tuple.hpp"
#include "list.hpp"

#include "parse.hpp"

#include "../ParseResult.hpp"
#include "../StringView.hpp"
#include "../strip.hpp"

#include <span>

namespace JutchsON {
    template <typename T, size_t i>
    ParseResult<T> setTuple(ParseResult<T> tuple, std::span<StringView> values, bool multiline) {
        if constexpr (i == std::tuple_size_v<T>) {
            return tuple;
        } else {
            using Value = std::tuple_element_t<i, T>;
            return setTuple<T, i + 1>(tuple.combine(parse<Value>(values[i], multiline ? Context::LINE : Context::OBJECT),
                [](const T& tuple, const Value& value) {
                T newTuple = tuple;
                std::get<i>(newTuple) = value;
                return newTuple;
            }), values, multiline);
        }
    }

    template <Tuplelike T>
    struct Parser<T> {
        ParseResult<T> operator() (StringView s, Context context) {
            if (context == Context::OBJECT) {
                if (auto stripped = strip(s); stripped.empty() || stripped.front() != '[') {
                    return ParseResult<T>::makeError(stripped.location(), "Expected a nested tuple");
                }
            }

            auto values = parseList(s);
            if (!values) {
                return values.errors();
            }

            if (std::ssize(*values) != std::tuple_size_v<T>) {
                return ParseResult<T>::makeError(s.location(),
                    std::format("Tuple size should be {} but it is {}", std::tuple_size_v<T>, std::ssize(*values)));
            }

            auto multiline = isMultilineList(s);
            if (!multiline) {
                return multiline.errors();
            }

            return setTuple<T, 0>({{}}, *values, *multiline);
        }
    };
}

#endif
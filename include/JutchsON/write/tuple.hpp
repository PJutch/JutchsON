#ifndef JUTCHSON_WRITE_TUPLE_HPP_
#define JUTCHSON_WRITE_TUPLE_HPP_

#include "../tuple.hpp"
#include "list.hpp"

#include "write.hpp"

#include <boost/describe.hpp>

#include <vector>

namespace JutchsON {
    template <typename T, size_t i>
    void tupleElements(const T& tuple, const auto& env, Context context, std::vector<std::string>& elements) {
        if constexpr (i >= std::tuple_size_v<T>) {
            return;
        } else {
            elements.push_back(write(get<i>(tuple), env, context));
            return tupleElements<T, i + 1>(tuple, env, context, elements);
        }
    }

    template <Tuplelike T>
    struct Writer<T> {
        std::string operator() (const T& tuple, const auto& env, Context context) {
            bool multiline = shouldBeMultiline(tuple);

            std::vector<std::string> elements;
            tupleElements<T, 0>(tuple, env, multiline ? Context::LINE : Context::OBJECT, elements);

            if (multiline) {
                return writeMultilineList(elements);
            } else {
                return writeOnelineList(elements, context == Context::OBJECT);
            }
        }
    };
}

#endif
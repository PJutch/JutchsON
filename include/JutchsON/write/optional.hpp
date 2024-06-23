#ifndef JUTCHSON_WRITE_OPTIONAL_HPP_
#define JUTCHSON_WRITE_OPTIONAL_HPP_

#include "write.hpp"

#include "../Context.hpp"

#include <optional>
#include <string_view>
#include <string>

namespace JutchsON {
    inline std::string writeOptional(std::string_view value) {
        bool quoted = !value.empty() && value.front() == '<' || value == "null" || value == "Null" || value == "NULL";
        if (!quoted) {
            return std::string{value};
        }

        std::string res;
        res.push_back('<');
        res.append(value);
        res.push_back('>');
        return res;

    }

    template <typename T>
    struct Writer<std::optional<T>> {
        std::string operator() (std::optional<T> v, Context context) {
            if (v) {
                return writeOptional(write(*v, context));
            } else {
                return "null";
            }
        }
    };
}

#endif
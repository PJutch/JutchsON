#ifndef JUTCHSON_WRITE_VARIANT_HPP_
#define JUTCHSON_WRITE_VARIANT_HPP_

#include "write.hpp"

#include "../Context.hpp"

#include <variant>
#include <string_view>
#include <string>

namespace JutchsON {
    inline std::string writeVariant(std::string_view type, std::string_view value, bool quoted = false) {
        std::string res;
        if (quoted) {
            res.push_back('<');
        }

        res.append(type);
        if (!value.empty()) {
            res.push_back(' ');
            res.append(value);
        }

        if (quoted) {
            res.push_back('>');
        }
        return res;
    }

    template <typename... Types>
    struct Writer<std::variant<Types...>> {
        template <typename Env>
        std::string operator() (std::variant<Types...> v, Env&& env, Context context) {
            return writeVariant(write(v.index(), env, Context::OBJECT), std::visit([&](const auto& val) {
                return write(val, std::forward<Env>(env), Context::LINE_REST);
            }, v), context == Context::OBJECT);
        }
    };
}

#endif
#ifndef JUTCHSON_PARSE_TYPE_HPP_
#define JUTCHSON_PARSE_TYPE_HPP_

#include "parse.hpp"
#include "../type.hpp"

#include "variant.hpp"

#include "../ParseResult.hpp"
#include "../StringView.hpp"

namespace JutchsON {
    template <typename GenericF, typename Typenames>
    using InvokeOnTagsResult = std::invoke_result_t<GenericF, typename std::tuple_element_t<0, Typenames>::TagType>;

    template <size_t i = 0, typename Typenames, typename GenericF>
    ParseResult<InvokeOnTagsResult<GenericF, Typenames>> parseType(
        StringView s, const Typenames& typenames, GenericF&& genericF
    ) {
        if constexpr (i >= std::tuple_size_v<Typenames>) {
            return ParseResult<InvokeOnTagsResult<GenericF, Typenames>>
                ::makeError(s.location(), std::format("Unknown type {}", s.asStd()));
        } else {
            if (s == std::get<i>(typenames).value) {
                return genericF(std::get<i>(typenames).tag());
            }
            return parseType<i + 1>(s, typenames, genericF);
        }
    }

    template <typename Base, typename Typenames, typename Env = EmptyEnv>
    ParseResult<std::unique_ptr<Base>> parseTypeVariant(StringView s, const Typenames& typenames, Env&& env = {}) {
        return parseVariant(s).then([&](auto pair) -> ParseResult<std::unique_ptr<Base>> {
            return joined(parseType(pair.first, typenames, [&](auto tag) {
                using Type = PayloadType<decltype(tag)>;
                return parse<Type>(pair.second, std::forward<Env>(env), Context::LINE_REST).map([&](const Type& value) {
                    return static_cast<std::unique_ptr<Base>>(std::make_unique<Type>(value));
                });
            }));
        });
    }
}

#endif
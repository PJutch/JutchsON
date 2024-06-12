#ifndef JUTCHSON_PARSE_HPP_
#define JUTCHSON_PARSE_HPP_

#include "ParseResult.hpp"
#include "StringView.hpp"
#include "Context.hpp"

namespace JutchsON {
    template <typename T>
    struct Parser;

    template <typename T>
    ParseResult<T> parse(StringView s, Context context = Context::LINE) {
        return Parser<T>{}(s, context);
    }
}

#endif
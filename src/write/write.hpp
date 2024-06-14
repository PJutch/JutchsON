#ifndef JUTCHSON_WRITE_HPP_
#define JUTCHSON_WRITE_HPP_

#include "Context.hpp"

#include <string>

namespace JutchsON {
    template <typename T>
    struct Writer;

    template <typename T>
    std::string write(T t, Context context = Context::LINE) {
        return Writer<T>{}(t, context);
    }
}

#endif
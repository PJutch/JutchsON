#ifndef JUTCHSON_WRITE_HPP_
#define JUTCHSON_WRITE_HPP_

#include "Context.hpp"
#include "filesystem.hpp"

#include <string>

namespace JutchsON {
    template <typename T>
    struct Writer;

    template <typename T>
    std::string write(T t, Context context = Context::LINE) {
        return Writer<T>{}(t, context);
    }

    template <typename T>
    void writeFile(const std::filesystem::path& path, T t) {
        writeWholeFile(path, write(t));
    }
}

#endif
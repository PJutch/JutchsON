#ifndef JUTCHSON_WRITE_HPP_
#define JUTCHSON_WRITE_HPP_

#include "../Context.hpp"
#include "../filesystem.hpp"

#include <string>

namespace JutchsON {
    template <typename T>
    struct Writer;

    template <typename T, typename Env = EmptyEnv>
    std::string write(T t, Env&& env = {}, Context context = Context::LINE) {
        return Writer<T>{}(t, std::forward<Env>(env), context);
    }

    template <typename T, typename Env = EmptyEnv>
    void writeFile(const std::filesystem::path& path, T t, Env&& env = {}) {
        writeWholeFile(path, write(t), std::forward<Env>(env));
    }
}

#endif
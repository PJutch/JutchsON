#ifndef JUTCHSON_PARSE_HPP_
#define JUTCHSON_PARSE_HPP_

#include "../ParseResult.hpp"
#include "../StringView.hpp"
#include "../Context.hpp"

#include "../filesystem.hpp"

#include <format>
#include <filesystem>
#include <utility>

namespace JutchsON {
    struct EmptyEnv {};

    template <typename T>
    struct Parser;

    template <typename T, typename Env = EmptyEnv>
    ParseResult<T> parse(StringView s, Env&& enviroment = {}, Context context = Context::LINE) {
        return Parser<T>{}(s, std::forward<Env>(enviroment), context);
    }

    template <typename T, typename Env = EmptyEnv>
    ParseResult<T> parseFile(const std::filesystem::path& path, Env&& env = {}) {
        if (!std::filesystem::exists(path)) {
            return ParseResult<T>::makeError({}, std::format("{} doesn't exists", path.generic_string()));
        }

        if constexpr (requires {
            Parser<T>{}(&path, std::forward<Env>(env));
        }) {
            return Parser<T>{}(&path, std::forward<Env>(env));
        }

        if (std::filesystem::is_directory(path)) {
            return ParseResult<T>::makeError({}, std::format("Expected a file, got directory {}", path.generic_string()));
        } else {
            return parse<T>(readWholeFile(path), std::forward<Env>(env));
        }
    }
}

#endif
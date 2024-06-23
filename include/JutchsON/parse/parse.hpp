#ifndef JUTCHSON_PARSE_HPP_
#define JUTCHSON_PARSE_HPP_

#include "../ParseResult.hpp"
#include "../StringView.hpp"
#include "../Context.hpp"

#include "../filesystem.hpp"

#include <format>
#include <filesystem>

namespace JutchsON {
    template <typename T>
    struct Parser;

    template <typename T>
    ParseResult<T> parse(StringView s, Context context = Context::LINE) {
        return Parser<T>{}(s, context);
    }

    template <typename T>
    ParseResult<T> parseFile(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) {
            return ParseResult<T>::makeError({}, std::format("{} doesn't exists", path.generic_string()));
        }

        if constexpr (requires {
            Parser<T>{}(&path);
        }) {
            return Parser<T>{}(&path);
        }

        if (std::filesystem::is_directory(path)) {
            return ParseResult<T>::makeError({}, std::format("Expected a file, got directory {}", path.generic_string()));
        } else {
            return parse<T>(readWholeFile(path));
        }
    }
}

#endif
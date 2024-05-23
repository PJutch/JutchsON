#ifndef JUTCHSON_PARSE_RESULT_HPP_
#define JUTCHSON_PARSE_RESULT_HPP_

#include "Location.hpp"

#include <variant>
#include <string>
#include <vector>
#include <ranges>
#include <concepts>

namespace JutchsON {
    struct ParseError {
        Location location;
        std::string what;

        friend bool operator == (const ParseError&, const ParseError&) = default;
    };

    template <typename T>
    class ParseResult {
    public:
        ParseResult(T&& value) : data{std::move(value)} {}
        ParseResult(const T& value) : data{value} {}

        template <typename Range> requires std::convertible_to<std::ranges::range_value_t<Range>, ParseError>
        ParseResult(Range&& errors) : data{std::vector<ParseError>(errors.begin(), errors.end())} {}

        static ParseResult<T> makeError(Location location, std::string what) {
            return {std::vector<ParseError>{{location, std::move(what)}}};
        }

        friend bool operator == (const ParseResult&, const ParseResult&) = default;
    private:
        std::variant<T, std::vector<ParseError>> data;
    };
}

#endif
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

        ParseResult<T> offseted(Location offset) const {
            if (const T* value = std::get_if<T>(&data)) {
                return *value;
            } else {
                std::vector<ParseError> errors = std::get<std::vector<ParseError>>(data);
                for (ParseError& error : errors) {
                    error.location = combine(error.location, offset);
                }
                return errors;
            }
        }

        template <typename UnaryF> requires std::convertible_to<std::invoke_result_t<UnaryF, T>, T>
        ParseResult<T> then(UnaryF&& f) {
            if (const T* value = std::get_if<T>(&data)) {
                return f(*value);
            } else {
                return *this;
            }
        }
    private:
        std::variant<T, std::vector<ParseError>> data;
    };
}

#endif
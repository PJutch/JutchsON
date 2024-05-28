#ifndef JUTCHSON_PARSE_RESULT_HPP_
#define JUTCHSON_PARSE_RESULT_HPP_

#include "Location.hpp"

#include <variant>
#include <format>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <concepts>
#include <stdexcept>

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

        T getOk() const {
            if (const T* value = std::get_if<T>(&data)) {
                return *value;
            } else {
                throw std::logic_error{"Expected parsing to be successful when it isn't"};
            }
        }

        ParseResult<T> rebased(Location base) const {
            if (const T* value = std::get_if<T>(&data)) {
                return *value;
            } else {
                std::vector<ParseError> errors = std::get<std::vector<ParseError>>(data);
                for (ParseError& error : errors) {
                    error.location = combine(base, error.location);
                }
                return errors;
            }
        }

        template <typename UnaryF>
        ParseResult<std::invoke_result_t<UnaryF, T>> map(UnaryF&& f) const {
            if (const T* value = std::get_if<T>(&data)) {
                return f(*value);
            } else {
                return std::get<std::vector<ParseError>>(data);
            }
        }

        template <typename UnaryF> requires std::constructible_from<std::invoke_result_t<UnaryF, T>, std::vector<ParseError>>
        std::invoke_result_t<UnaryF, T> then(UnaryF&& f) const {
            if (const T* value = std::get_if<T>(&data)) {
                return f(*value);
            } else {
                return std::get<std::vector<ParseError>>(data);
            }
        }

        const std::variant<T, std::vector<ParseError>>& variant() const {
            return data;
        }
    private:
        std::variant<T, std::vector<ParseError>> data;
    };

    template <typename T>
    concept Printable = requires (T value, std::ostream & os) {
        { os << value } -> std::convertible_to<std::ostream&>;
    };

    template <Printable T> 
    std::ostream& operator << (std::ostream& os, const ParseResult<T>& res) {
        if (const T* value = std::get_if<T>(&res.variant())) {
            return os << *value;
        } else {
            const auto& errors = std::get<std::vector<ParseError>>(res.variant());
            os << "Parse failed with " << std::ssize(errors) << " errors" << std::endl;
            for (const auto& [location, what] : errors) {
                os << location.line << ':' << location.column << ' ' << what << std::endl;
            }
            return os;
        } 
    }
}

#endif
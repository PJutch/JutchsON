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

        template <typename U> requires !std::same_as<U, ParseResult<T>> && std::equality_comparable_with<U, T>
        friend bool operator == (const ParseResult& result, const U& value) {
            return result && *result == value;
        }

        template <typename U> requires !std::same_as<U, ParseResult<T>> && std::equality_comparable_with<U, T>
        friend bool operator == (const U& value, const ParseResult& result) {
            return result == value;
        }

        explicit operator bool() const {
            return std::holds_alternative<T>(data);
        }

        T& operator *() {
            if (T* value = std::get_if<T>(&data)) {
                return *value;
            } else {
                throw std::logic_error{"Expected parsing to be successful when it isn't"};
            }
        }

        const T& operator *() const {
            if (const T* value = std::get_if<T>(&data)) {
                return *value;
            } else {
                throw std::logic_error{"Expected parsing to be successful when it isn't"};
            }
        }

        T* operator->() {
            if (T* value = std::get_if<T>(&data)) {
                return value;
            } else {
                throw std::logic_error{"Expected parsing to be successful when it isn't"};
            }
        }

        const T* operator->() const {
            if (const T* value = std::get_if<T>(&data)) {
                return value;
            } else {
                throw std::logic_error{"Expected parsing to be successful when it isn't"};
            }
        }

        const std::vector<ParseError>& errors() const {
            if (const auto* errors = std::get_if<std::vector<ParseError>>(&data)) {
                return *errors;
            } else {
                throw std::logic_error{"Expected parsing to not be successful when it is"};
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

        template <typename U, typename BinaryF>
        ParseResult<std::invoke_result_t<BinaryF, T, U>> combine(const ParseResult<U>& other, BinaryF&& f) const {
            if (*this && other) {
                return f(**this, *other);
            } else if (*this) {
                return other.errors();
            } else if (other) {
                return errors();
            } else {
                std::vector<ParseError> combinedErrors;
                combinedErrors.reserve(std::ssize(errors()) + std::ssize(other.errors()));

                std::ranges::copy(errors(), std::back_inserter(combinedErrors));
                std::ranges::copy(other.errors(), std::back_inserter(combinedErrors));

                return combinedErrors;
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
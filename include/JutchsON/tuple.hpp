#ifndef JUTCHSON_TUPLE_HPP_
#define JUTCHSON_TUPLE_HPP_

#include <tuple>

namespace JutchsON {
    template <typename T, size_t i>
    struct HasTupleInterface {
        static inline constexpr bool value = requires (T t) {
            typename std::tuple_element_t<i - 1, T>;
            { get<i - 1>(t) } -> std::convertible_to<std::tuple_element_t<i - 1, T>>;
        } && HasTupleInterface<T, i - 1>::value;
    };

    template <typename T>
    struct HasTupleInterface<T, 0> {
        static inline constexpr bool value = true;
    };

    template <typename T>
    concept Tuplelike = requires {
        std::tuple_size<T>::value;
    } && HasTupleInterface<T, std::tuple_size_v<T>>::value;
}

#endif
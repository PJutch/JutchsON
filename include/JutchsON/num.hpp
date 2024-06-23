#ifndef JUTCHSON_NUM_HPP_
#define JUTCHSON_NUM_HPP_

#include <limits>

namespace JutchsON {
    template <typename T>
    concept UnsignedInteger = std::numeric_limits<T>::is_integer && !std::numeric_limits<T>::is_signed;

    template <typename T>
    concept SignedInteger = std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed;

    template <typename T>
    concept UnsignedFloat = std::numeric_limits<T>::is_specialized
        && !std::numeric_limits<T>::is_integer && !std::numeric_limits<T>::is_signed;

    template <typename T>
    concept SignedFloat = !std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed;
}

#endif
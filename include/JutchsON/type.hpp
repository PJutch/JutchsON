#ifndef JUTCHSON_TYPE_HPP_
#define JUTCHSON_TYPE_HPP_

#include <type_traits>

namespace JutchsON {
    template <typename T, typename Value = void>
    struct TaggedValue {
        using type = T;
        using TagType = TaggedValue<T>;

        Value value;

        TagType tag() const {
            return {};
        }
    };

    template <typename T>
    struct TaggedValue<T, void> {
        using type = T;
    };

    template <typename T>
    using PayloadType = typename std::remove_cvref_t<T>::type;
}

#define JUTCHSON_TAGGED_TYPE_NAME(T) (JutchsON::TaggedValue<T, std::string>{#T})

#endif
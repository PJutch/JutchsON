#ifndef JUTCHSON_WRITE_BOOL_HPP_
#define JUTCHSON_WRITE_BOOL_HPP_

#include <string>

namespace JutchsON {
    inline std::string writeBool(bool b, bool allowEmpty = false) {
        if (b) {
            return allowEmpty ? "" : "true";
        } else {
            return "false";
        }
    }

    template <>
    struct Writer<bool> {
        std::string operator() (bool b, Context context) {
            return writeBool(b, context == Context::LINE_REST);
        }
    };

    template <>
    struct forcesMultilineImpl<bool> {
        static inline constexpr bool value = false;
    };
}

#endif
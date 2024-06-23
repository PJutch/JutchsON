#ifndef JUTCHSON_LOCATION_HPP_
#define JUTCHSON_LOCATION_HPP_

#include <string_view>
#include <cstdint>

namespace JutchsON {
    struct Location {
        ptrdiff_t line;
        ptrdiff_t column;

        friend bool operator == (Location, Location) = default;

        static Location fromIndex(std::string_view s, ptrdiff_t index) {
            ptrdiff_t line = 0;
            ptrdiff_t last_newline = -1;
            for (ptrdiff_t i = 0; i < index; ++i) {
                if (s[i] == '\n') {
                    ++line;
                    last_newline = i;
                }
            }
            return {line, index - last_newline - 1};
        }

        static Location fromIndex(std::string_view s, Location base, ptrdiff_t index);
    };

    inline Location combine(Location base, Location offset) {
        if (offset.line == 0) {
            return {base.line, base.column + offset.column};
        } else {
            return {base.line + offset.line, offset.column};
        }
    }

    inline Location Location::fromIndex(std::string_view s, Location base, ptrdiff_t index) {
        return combine(base, fromIndex(s, index));
    }
}

#endif
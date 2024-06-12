#ifndef JUTCHSON_WRITE_BOOL_HPP_
#define JUTCHSON_WRITE_BOOL_HPP_

#include <string>

namespace JutchsON {
    inline std::string writeBool(bool s, bool allowEmpty = false) {
        if (s) {
            return allowEmpty ? "" : "true";
        } else {
            return "false";
        }
    }
}

#endif
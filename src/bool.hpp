#ifndef JUTCHSON_BOOL_HPP_
#define JUTCHSON_BOOL_HPP_

#include "ParseResult.hpp"
#include "StringView.hpp"

#include <format>
#include <string>
#include <algorithm>

namespace JutchsON {
    inline ParseResult<bool> parseBool(StringView s, bool allowEmpty = false) {
        s = strip(s);

        if (s.empty()) {
            return allowEmpty ? true : ParseResult<bool>::makeError(s.location(), "Empty bool isn't allowed");
        }

        if (s == "true" || s == "True" || s == "TRUE") {
            return true;
        } else if (s == "false" || s == "False" || s == "FALSE") {
            return false;
        } else {
            return ParseResult<bool>::makeError(s.location(), std::format("\"{}\" is invalid bool value", s.asStd()));
        }
    }

    inline std::string writeBool(bool s, bool allowEmpty = false) {
        if (s) {
            return allowEmpty ? "" : "true";
        } else {
            return "false";
        }
    }
}

#endif
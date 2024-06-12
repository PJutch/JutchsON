#ifndef JUTCHSON_WRITE_DICT_HPP_
#define JUTCHSON_WRITE_DICT_HPP_

#include "object.hpp"

#include <vector>
#include <utility>

namespace JutchsON {
    inline std::string writeDict(std::span<const std::pair<std::string, std::string>> d) {
        std::string res;
        res.push_back('{');

        for (const std::pair<std::string, std::string>& pair : d) {
            res.push_back('\n');
            res.append(indent(pair.first));

            if (!pair.second.empty()) {
                res.push_back(' ');
                std::string indented = indent(pair.second);
                res.append(strip(std::string_view{indented}).asStd());
            }
        }

        if (res.back() != '{') {
            res.push_back('\n');
        }
        res.push_back('}');
        return res;
    }
}

#endif
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

    template <typename Key, typename Value>
    struct Writer<std::unordered_multimap<Key, Value>> {
        std::string operator() (const std::unordered_multimap<Key, Value>& map, Context context) {
            std::vector<std::pair<std::string, std::string>> pairs;
            pairs.reserve(std::ssize(map));
            for (const auto& [key, value] : map) {
                pairs.emplace_back(write(key, Context::OBJECT), write(value, Context::LINE_REST));
            }

            return writeDict(pairs);
        }
    };

    template <typename Key, typename Value>
    struct Writer<std::unordered_map<Key, Value>> {
        std::string operator() (const std::unordered_map<Key, Value>& map, Context context) {
            std::vector<std::pair<std::string, std::string>> pairs;
            pairs.reserve(std::ssize(map));
            for (const auto& [key, value] : map) {
                pairs.emplace_back(write(key, Context::OBJECT), write(value, Context::LINE_REST));
            }

            return writeDict(pairs);
        }
    };

    template <Described T>
    struct Writer<T> {
        std::string operator() (T t, Context context) {
            std::vector<std::pair<std::string, std::string>> pairs;

            boost::mp11::mp_for_each<boost::describe::describe_members<T, boost::describe::mod_public>>([&](auto d) {
                using ValueT = std::remove_cvref_t<decltype(t.*d.pointer)>;

                bool shouldWrite = true;
                if constexpr (std::default_initializable<T>) {
                    shouldWrite = t.*d.pointer != T{}.*d.pointer;
                }

                if (shouldWrite) {
                    pairs.emplace_back(write(d.name), write(t.*d.pointer));
                }
            });

            return writeDict(pairs);
        }
    };
}

#endif
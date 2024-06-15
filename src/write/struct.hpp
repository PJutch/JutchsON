#ifndef JUTCHSON_WRITE_STRUCT_HPP_
#define JUTCHSON_WRITE_STRUCT_HPP_

#include "struct.hpp"
#include "write/dict.hpp"

#include "write.hpp"

#include <vector>
#include <utility>

namespace JutchsON {
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
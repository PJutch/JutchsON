#ifndef JUTCHSON_STRUCT_HPP_
#define JUTCHSON_STRUCT_HPP_

#include <boost/describe.hpp>

namespace JutchsON {
    template <typename T>
    concept Described = requires {
        typename boost::describe::describe_members<T, boost::describe::mod_public>;
    };
}

#endif
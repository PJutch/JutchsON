#ifndef JUTCHSON_WRITE_NUM_HPP_
#define JUTCHSON_WRITE_NUM_HPP_

#include <algorithm>
#include <string>
#include <cmath>

namespace JutchsON {
    template <typename T>
    std::string writeUint(T n) {
        if (n == 0) {
            return "0";
        }

        std::string res;
        while (n > 0) {
            res.push_back('0' + n % 10);
            n /= 10;
        }

        std::ranges::reverse(res);
        return res;
    }

    template <typename T>
    std::string writeInt(T n) {
        if (n < 0) {
            return '-' + writeUint(-n);
        }
        return writeUint(n);
    }

    template <typename T>
    std::string writeNonnegativeFloat(T x) {
        if (isnan(x)) {
            return "nan";
        } else if (isinf(x)) {
            return "inf";
        }

        std::string res;

        T intpart;
        T fractpart = modf(x, &intpart);

        if (intpart == 0) {
            res.push_back('0');
        }

        while (intpart > 0) {
            res.push_back('0' + static_cast<ptrdiff_t>(fmod(intpart, 10)));

            intpart /= 10;
            modf(intpart, &intpart);
        }

        std::ranges::reverse(res);
        res.push_back('.');

        for (ptrdiff_t i = 0; i < 18; ++i) {
            fractpart *= 10;

            T digit;
            fractpart = modf(fractpart, &digit);
            res.push_back('0' + static_cast<ptrdiff_t>(digit));
        }

        while (res.back() == '0') {
            res.pop_back();
        }

        if (res.back() == '.') {
            res.pop_back();
        }

        return res;
    }

    template <typename T>
    std::string writeFloat(T x) {
        if (std::signbit(x)) {
            return '-' + writeNonnegativeFloat(-x);
        }
        return writeNonnegativeFloat(x);
    }
}

#endif
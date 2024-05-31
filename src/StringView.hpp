#ifndef JUTCHSON_STRING_VIEW_HPP_
#define JUTCHSON_STRING_VIEW_HPP_

#include "Location.hpp"

#include <string_view>
#include <algorithm>
#include <iterator>
#include <concepts>
#include <cstdint>

namespace JutchsON {
    class StringView {
    public:
        class iterator {
        public:
            using value_type = char;
            using iterator_category = std::contiguous_iterator_tag;

            iterator() : base{}, baseStr{} {}
            iterator(std::string_view::iterator iter, std::string_view s) : base{iter}, baseStr{s} {}

            iterator& operator ++() {
                ++base;
                return *this;
            }

            iterator operator++ (int) {
                auto old = *this;
                ++*this;
                return old;
            }

            iterator& operator --() {
                --base;
                return *this;
            }

            iterator operator-- (int) {
                auto old = *this;
                --*this;
                return old;
            }

            const char& operator *() const {
                return *base;
            }

            const char* operator->() const {
                return base.operator->();
            }

            friend bool operator == (iterator lhs, iterator rhs) {
                return lhs.base == rhs.base;
            }

            friend auto operator <=> (iterator lhs, iterator rhs) {
                return lhs.base <=> rhs.base;
            }

            friend iterator operator + (iterator iter, ptrdiff_t offset) {
                return iterator{iter.base + offset, iter.baseStr};
            }

            friend iterator operator + (ptrdiff_t offset, iterator iter) {
                return iter + offset;
            }

            iterator& operator += (ptrdiff_t offset) {
                return *this = *this + offset;
            }

            friend iterator operator - (iterator iter, ptrdiff_t offset) {
                return iter + (-offset);
            }

            iterator& operator -= (ptrdiff_t offset) {
                return *this = *this - offset;
            }

            friend ptrdiff_t operator - (iterator lhs, iterator rhs) {
                return lhs.base - rhs.base;
            }

            const char& operator[] (ptrdiff_t i) const {
                return base[i];
            }

            std::string_view baseString() const {
                return baseStr;
            }

            ptrdiff_t offset() const {
                return base - baseStr.begin();
            }
        private:
            std::string_view::iterator base;
            std::string_view baseStr;
        };

        StringView() : base{}, offset{0}, len{0} {}

        StringView(std::string_view s) : base{s}, offset{0}, len{std::ssize(base)} {}

        StringView(iterator begin, iterator end) : base{begin.baseString()}, offset{begin.offset()}, len{end - begin} {}

        StringView(const char* s) : base{s}, offset{0}, len{std::ssize(base)} {}

        iterator begin() const {
            return {base.begin() + offset, base};
        }

        iterator end() const {
            return {base.begin() + offset + len, base};
        }

        auto cbegin() const {
            return begin();
        }

        auto cend() const {
            return end();
        }

        auto rbegin() const {
            return std::reverse_iterator{end()};
        }

        auto rend() const {
            return std::reverse_iterator{begin()};
        }

        auto crbegin() const {
            return rbegin();
        }

        auto crend() const {
            return rend();
        }

        size_t size() const {
            return len;
        }

        bool empty() const {
            return base.empty();
        }

        const char* data() const {
            return base.data() + offset;
        }

        char operator[] (ptrdiff_t i) const {
            return base[offset + i];
        }

        char front() const {
            return base[offset];
        }

        char back() const {
            return base[offset + len - 1];
        }

        friend bool operator == (const StringView& lhs, const StringView& rhs) {
            return std::ranges::equal(lhs, rhs);
        }
        
        void remove_prefix(ptrdiff_t n) {
            offset += n;
            len -= n;
        }

        void remove_suffix(ptrdiff_t n) {
            len -= n;
        }

        StringView substr(int64_t pos, int64_t count = std::string_view::npos) {
            if (count == std::string_view::npos) {
                return {begin() + pos, end()};
            }
            return {begin() + pos, begin() + std::min(len, pos + count)};
        }

        Location location(int64_t pos = 0) const {
            return Location::fromIndex(base, offset + pos);
        }

        std::string_view asStd() const {
            return base.substr(offset, len);
        }
    private:
        std::string_view base;
        ptrdiff_t offset;
        ptrdiff_t len;
    };
}

template <>
inline constexpr bool std::ranges::enable_borrowed_range<JutchsON::StringView> = true;

#endif
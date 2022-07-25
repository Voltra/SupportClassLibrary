#pragma once

#include <chrono>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace scl {
    namespace alias {
        // Containers
        /**
         * A collection of key-value pairs
         * @tparam Key being the type of the keys
         * @tparam Value being the type of the values
         * @tparam Hasher being the type of the hasher function for keys
         * @tparam KeyEq being the type of the equality comparison of keys
         * @tparam Allocator being the type of allocator to use for pairs
         */
        template <class Key, class Value, class Hasher = std::hash<Key>,
                  class KeyEq = std::equal_to<Key>,
                  class Allocator = std::allocator<std::pair<const Key, Value>>>
        using Map = std::unordered_map<Key, Value, Hasher, KeyEq, Allocator>;

        /**
         * A sorted collection of key-value pairs
         * @tparam Key being the type of the keys (used for sorting)
         * @tparam Value being the type of the values
         */
        template <class Key, class Value, class Comparator = std::less<Key>,
                  class Allocator = std::allocator<std::pair<const Key, Value>>>
        using SortedMap = std::map<Key, Value, Comparator, Allocator>;

        /**
         * A set of unique values
         * @tparam Value being the type of values stored
         */
        template <class Value, class Hasher = std::hash<Value>, class KeyEq = std::equal_to<Value>,
                  class Allocator = std::allocator<Value>>
        using Set = std::unordered_set<Value, Hasher, KeyEq, Allocator>;

        /**
         * A sorted set of unique values
         * @tparam Value being the type of values stored (used for sorting)
         */
        template <class Value, class Comparator = std::less<Key>,
                  class Allocator = std::allocator<Value>>
        using SortedSet = std::set<Value, Comparator, Allocator>;

        // Numbers
        /**
         * Unsigned character type
         */
        using uchar = unsigned char;

        /**
         * Unsigned short integer
         */
        using ushort = unsigned short;

        /**
         * Unsigned integer
         */
        using uint = unsigned int;

        /**
         * Unsigned long integer
         */
        using ulong = unsigned long;

        /**
         * Unsigned long long integer
         */
        using ullong = unsigned long long;

        /**
         * Unsigned size type
         */
        using usize = std::size_t;

        /**
         * 8-bit unsigned integer
         */
        using u8 = uint8_t;

        /**
         * 16-bit unsigned integer
         */
        using u16 = uint16_t;

        /**
         * 32-bit unsigned integer
         */
        using u32 = uint32_t;

        /**
         * 64-bit unsigned integer
         */
        using u64 = uint64_t;

        /**
         * 8-bit integer
         */
        using i8 = int8_t;

        /**
         * 16-bit integer
         */
        using i16 = int16_t;

        /**
         * 32-bit integer
         */
        using i32 = int32_t;

        /**
         * 64-bit integer
         */
        using i64 = int64_t;

        /**
         * 16-bit character
         */
        using c16 = char16_t;

        /**
         * 32-bit character
         */
        using c32 = char32_t;

        /**
         * Long double
         */
        using ldouble = long double;

        /**
         * Duration in nanoseconds
         */
        using nanoseconds = std::chrono::nanoseconds;

        /**
         * Duration in microseconds
         */
        using microseconds = std::chrono::microseconds;

        /**
         * Duration in milliseconds
         */

        using milliseconds = std::chrono::milliseconds;

        /**
         * Duration in seconds
         */

        using seconds = std::chrono::seconds;

        /**
         * Duration in minutes
         */
        using minutes = std::chrono::minutes;

        /**
         * Duration in hours
         */
        using hours = std::chrono::hours;

#ifdef SCL_CPP20
        /**
         * Duration in days
         */
        using days = std::chrono::days;

        /**
         * Duration in weeks
         */
        using weeks = std::chrono::weeks;

        /**
         * Duration in months
         */
        using months = std::chrono::months;

        /**
         * Duration in years
         */
        using years = std::chrono::years;
#endif
    }  // namespace alias
}  // namespace scl
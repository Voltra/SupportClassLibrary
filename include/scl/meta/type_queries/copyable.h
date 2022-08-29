#pragma once
#include "./stl.h"

namespace scl {
    namespace meta {
        /**
         * Determines whether a type defines copy semantics
         * @tparam T being the type to check against
         * @return TRUE if it defines copy semantics, FALSE otherwise
         */
        template <class T>
        inline constexpr bool is_copyable() {
            return is_copy_assignable<T>() && is_copy_constructible<T>();
        }

        //        /**
        //         * Determines whether a type can be trivially copied
        //         * @tparam T being the type to check against
        //         * @return TRUE if it defines trivial copy semantics, FALSE otherwise
        //         */
        //        template <class T> inline constexpr bool is_trivially_copyable() {
        //            return is_trivially_copy_assignable<T>() &&
        //            is_trivially_copy_constructible<T>();
        //        }

        /**
         * Determines whether a type can be copied without exceptions
         * @tparam T being the type to check against
         * @return TRUE if it defines nothrow copy semantics, FALSE otherwise
         */
        template <class T>
        inline constexpr bool is_nothrow_copyable() {
            return is_nothrow_copy_assignable<T>() && is_nothrow_copy_constructible<T>();
        }
    }  // namespace meta
}  // namespace scl
#pragma once
#include "./stl.h"

namespace scl {
    namespace meta {
        /**
         * Determines whether a type defines move semantics
         * @tparam T being the type to check against
         * @return TRUE if it defines move semantics, FALSE otherwise
         */
        template <class T>
        inline constexpr bool is_movable() {
            return is_move_assignable<T>() && is_move_constructible<T>();
        }

        /**
         * Determines whether a type can be trivially moved
         * @tparam T being the type to check against
         * @return TRUE if it defines trivial move semantics, FALSE otherwise
         */
        template <class T>
        inline constexpr bool is_trivially_movable() {
            return is_trivially_move_assignable<T>() && is_trivially_move_constructible<T>();
        }

        /**
         * Determines whether a type can be moved without exceptions
         * @tparam T being the type to check against
         * @return TRUE if it defines nothrow move semantics, FALSE otherwise
         */
        template <class T>
        inline constexpr bool is_nothrow_movable() {
            return is_nothrow_move_assignable<T>() && is_nothrow_move_constructible<T>();
        }
    }  // namespace meta
}  // namespace scl
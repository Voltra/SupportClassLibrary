#pragma once
#include <type_traits>
#include <utility>

#include "../type_aliases/stl.h"
#include "../type_aliases/void_t.h"
#include "./exists.h"
#include "./stl.h"

namespace scl {
    namespace meta {
        namespace details {
            template <class T, class U, class = void>
            struct swappable_with : std::false_type {};

#define SCL_t std::declval<T&>()
#define SCL_u std::declval<U&>()
            template <class T, class U>
            struct swappable_with<T, U,
                                  scl::meta::void_enable_if_t<
                                      scl::meta::exists<decltype(std::swap(SCL_t, SCL_u))>()
                                      && scl::meta::exists<decltype(std::swap(SCL_u, SCL_t))>()>>
                : std::true_type {};

#undef SCL_u
#undef SCL_t
        }  // namespace details

        template <class T, class U>
        inline constexpr bool is_swappable_with() {
            return details::swappable_with<T, U>::value;
        }

        namespace details {
            template <class T, class = void>
            struct swappable : std::false_type {};

            template <class T>
            struct swappable<T, scl::meta::void_enable_if_t<
                                    !scl::meta::is_same<T, scl::meta::add_lvalue_reference_t<T>>()
                                    && is_swappable_with<T&, T&>()>> : std::true_type {};
        }  // namespace details

        template <class T>
        inline constexpr bool is_swappable() {
            return details::swappable<T>::value;
        }
    }  // namespace meta
}  // namespace scl
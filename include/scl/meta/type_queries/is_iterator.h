#pragma once
#include "../type_aliases/stl.h"
#include "./stl.h"

namespace scl {
    namespace meta {
        namespace details {
            template <typename T, typename = void>
            struct is_iterator {
                static constexpr bool value = false;
            };

            template <typename T>
            struct is_iterator<T, scl::meta::enable_if_t<!scl::meta::is_same<
                                      typename std::iterator_traits<T>::value_type, void>()>> {
                static constexpr bool value = true;
            };
        }  // namespace details

        template <class T>
        static constexpr bool is_iterator() {
            return details::is_iterator<T>::value;
        }
    }      // namespace meta
}  // namespace scl
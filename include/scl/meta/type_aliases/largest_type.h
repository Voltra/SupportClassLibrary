#pragma once
#include "./stl.h"

namespace scl {
    namespace meta {
        namespace details {
            /**
             * @see https://stackoverflow.com/a/68879837/7316365
             * @tparam T
             * @tparam Ts
             */
            template <class T, class... Ts>
            struct largest_type {
                using type =
                    typename scl::meta::conditional_t<sizeof(T)
                                                          == std::max({sizeof(T), sizeof(Ts)...}),
                                                      T, typename largest_type<Ts...>::type>;
            };

            template <class T>
            struct largest_type<T> {
                using type = T;
            };
        }  // namespace details

        template <class T, class... Ts>
        using largest_type_t = typename details::largest_type<T, Ts...>::type;
    }  // namespace meta
}  // namespace scl

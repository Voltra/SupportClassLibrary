#pragma once
#include <utility>

#include "../../macros.h"
#include "./stl.h"

namespace scl {
    namespace meta {
        namespace details {
            template <class>
            struct as_const;

            template <class T>
            struct as_const {
                using type = const T&;
            };

            template <class T>
            struct as_const<T*> {
                using type = const T*;
            };

            template <class T>
            struct as_const<T&&> {
                using type = const T&;
            };
        }

        template <class T>
        using as_const_t = typename details::as_const<T>::type;

        template <class T>
        constexpr auto as_const(T&& value)
            -> SCL_RETURN(static_cast<as_const_t<T>>(std::forward<T>(value)))
    }
}  // namespace scl

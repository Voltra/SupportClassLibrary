#pragma once
#include "./stl.h"

namespace scl {
    namespace meta {
        /**
         * The must have
         */
        template <class...>
        using void_t = void;

        template <bool B, class T = void>
        using void_enable_if_t = void_t<enable_if_t<B, T>>;
    }  // namespace meta
}  // namespace scl
#pragma once
#include <utility>
#include "../../macros.h"
#include "./stl.h"

namespace scl {
    namespace meta {
        template <class T>
        constexpr auto as_const(T&& value) -> SCL_RETURN(
            static_cast<scl::meta::add_const_t<T>>(std::forward<T>(value))
        )
    }
}

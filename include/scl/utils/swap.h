#pragma once
#include <utility>

#include "../meta/type_aliases/stl.h"
#include "../meta/type_queries/swap.h"
#include "./exchange.h"

namespace scl {
    namespace utils {
        template <class T, class U>
        scl::meta::enable_if_t<scl::meta::is_swappable_with<T, U>(), void> swap(T& lhs, U& rhs) {
            lhs = exchange(rhs, lhs);
        }
    }  // namespace utils
}  // namespace scl
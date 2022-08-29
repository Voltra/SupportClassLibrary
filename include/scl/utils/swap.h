#pragma once
#include <utility>
#include "./exchange.h"

namespace scl {
    namespace utils {
        template <class T, class U>
        void swap(T& lhs, U& rhs) {
            lhs = exchange(rhs, lhs);
        }
    }
}
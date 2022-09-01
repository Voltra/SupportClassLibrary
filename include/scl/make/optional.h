#pragma once
#include <utility>

#include "../utils/Optional.h"

namespace scl {
    namespace make {
        template <class T, class... Args>
        inline constexpr scl::utils::Optional<T> optional(Args&&... args) {
            return scl::utils::Optional<T>::inplace(std::forward<Args>(args)...);
        }
    }  // namespace make
}  // namespace scl
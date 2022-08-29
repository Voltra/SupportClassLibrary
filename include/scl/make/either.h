#pragma once
#include "../utils/Either.h"

namespace scl {
    namespace make {
        template <class Left, class Right, class... Args>
        scl::utils::Either<Left, Right> left(Args&&... args) {
            return scl::utils::Either<Left, Right>::Left(std::forward<Args>(args)...);
        }

        template <class Left, class Right, class... Args>
        scl::utils::Either<Left, Right> right(Args&&... args) {
            return scl::utils::Either<Left, Right>::Right(std::forward<Args>(args)...);
        }
    }  // namespace make
}  // namespace scl
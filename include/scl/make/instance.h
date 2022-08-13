#pragma once
#include <utility>

namespace scl {
    namespace make {
        /**
         * Make an instance of T
         * @tparam T
         * @tparam Args
         * @param args
         * @return
         */
        template <class T, class... Args>
        inline T instance(Args&&... args) {
            return T{std::forward<Args>(args)...};
        }
    }  // namespace make
}  // namespace scl
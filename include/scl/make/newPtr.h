#pragma once
#include <utility>

namespace scl {
    namespace make {
        /**
         * Make a pointer using `new`
         * @tparam T
         * @tparam Args
         * @param args
         * @return
         */
        template <class T, class... Args>
        T* newPtr(Args&&... args) {
            return new T{std::forward<Args>(args)...};
        }
    }  // namespace make
}  // namespace scl
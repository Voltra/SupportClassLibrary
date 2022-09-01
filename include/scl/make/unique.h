#pragma once
#include <memory>
#include <utility>

#include "./newPtr.h"

namespace scl {
    namespace make {
        template <class Base, class Derived = Base, class... Args>
        std::unique_ptr<Base> unique(Args&&... args) {
            /*
             * Raw pointer + constructor instead of make_shared to reduce
             * the number of template instantiations
             */
            return std::unique_ptr<Base>{newPtr<Derived>(std::forward<Args>(args)...)};
        }
    }  // namespace make
}  // namespace scl
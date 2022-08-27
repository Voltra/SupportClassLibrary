#pragma once
#include <memory>
#include <utility>
#include "./newPtr.h"

namespace scl {
    namespace make {
        template <class Base, class Derived = Base, class... Args>
        std::unique_ptr<Base> unique(Args&&... args) {
            return std::unique_ptr<Base>{
                newPtr<Derived>(std::forward<Args>(args)...)
            };
        }
    }
}
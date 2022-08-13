#pragma once
#include <utility>
#include <memory>
#include "./"

namespace scl {
    namespace make {
        template <class T, class... Args>
        std::shared_ptr<T> shared(Args&&... args) {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    }
}
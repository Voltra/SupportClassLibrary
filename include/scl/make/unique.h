#pragma once
#include <memory>
#include <utility>
#include "./newPtr.h"

namespace scl {
    namespace make {
        template <class T, class... Args>
        std::unique_ptr<T> unique(Args&&... args) {
            return std::unique_ptr<T>{
                newPtr<T>(std::forward<Args>(args)...)
            };
        }
    }
}
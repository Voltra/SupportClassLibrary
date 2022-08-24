#pragma once
#include <utility>

#include "../utils/RValueForwarder.h"
#include "../utils/CaptureRValue.h"

namespace scl {
    namespace make {
        template <class T>
        constexpr scl::utils::RValueForwarder<T> rvalue(T&& value) {
            return scl::utils::RValueForwarder<T>{std::move(value)};
        }

        template <class T, class F>
        constexpr scl::utils::CaptureRValue<T, F> rvalueCapture(T&& value, F&& func) {
            return scl::utils::CaptureRValue<T, F>{std::forward<T>(value), std::forward<F>(func)};
        }
    }  // namespace make
}  // namespace scl
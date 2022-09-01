#pragma once
#include <utility>

#include "../macros.h"
#include "./invoke.h"

namespace scl {
    namespace utils {
        /**
         * @note see https://stackoverflow.com/a/20669290/7316365
         * @tparam T
         * @tparam F
         */
        template <class T, class F>
        class CaptureRValue {
            private:
                T value;
                F func;

            public:
                constexpr CaptureRValue(T&& val, F&& f)
                    : value{std::move(val)}, func{std::move(f)} {}

                template <class... Args>
                constexpr auto operator()(Args&&... args)
                    -> SCL_RETURN(invoke(func, std::move(value), std::forward<Args>(args)...))
        };
    }  // namespace utils
}  // namespace scl
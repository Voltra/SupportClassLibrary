#pragma once
#include <cassert>
#include <utility>

#include "./exchange.h"

namespace scl {
    namespace utils {
        /**
         * @note see https://stackoverflow.com/a/20669290/7316365
         * @tparam T
         */
        template <class T>
        class RValueForwarder {
            private:
                T value;
                bool wasMoved = false;

            public:
                RValueForwarder() = delete;

                explicit constexpr RValueForwarder(T&& val) : value{std::move(val)} {}

                constexpr RValueForwarder(RValueForwarder& rhs)
                    : value{std::move(rhs.value)}, wasMoved{true} {
                    assert(!rhs.wasMoved);
                }

                constexpr RValueForwarder(RValueForwarder&& rhs)
                    : value{std::move(rhs.value)}, wasMoved{exchange(rhs.wasMoved, true)} {}

                RValueForwarder& operator=(RValueForwarder&) = delete;
                RValueForwarder& operator=(RValueForwarder&&) = delete;

                constexpr T&& move() {
                    wasMoved = true;
                    return std::move(value);
                }
        };
    }  // namespace utils
}  // namespace scl
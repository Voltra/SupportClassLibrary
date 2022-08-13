#pragma once
#include "../macros.h"

namespace scl {
    namespace async {
        /**
         * Traits for use with the scl::async::with function
         * @tparam Resource being the type of the value passed to scl::async::with
         */
        template <class Resource>
        struct with_traits {
            /**
             * Invoke a delegate using the wrapper
             * @tparam F being the type of the delegate
             * @param resource being the resource to use
             * @param delegate being the delegate to which the value will be passed
             */
            template <class F>
            auto operator()(SCL_MAYBE_UNUSED Resource& resource, F&& delegate)
                -> SCL_RETURN(std::forward<F>(delegate)())
        };

        template <>
        struct with_traits<std::mutex> {
            template <class F>
            auto operator()(std::mutex& mutex, F&& delegate)
                -> decltype(std::forward<F>(delegate)()) {
                SCL_MAYBE_UNUSED std::lock_guard<decltype(mutex)> g{mutex};
                return std::forward<F>(delegate)();
            }
        };
    }  // namespace async
}  // namespace scl
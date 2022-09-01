#pragma once
#include <memory>
#include <mutex>
#include <utility>

#include "../macros.h"
#include "./with_traits.h"

namespace scl {
    namespace async {
        template <class T, class Lock = std::mutex, class Guard = std::lock_guard<Lock>>
        class Mutexed {
            public:
                using lock_type = Lock;
                using guard_type = Guard;
                using value_type = T;

            protected:
                value_type value;
                mutable std::unique_lock<lock_type> lock{lock_type{}, std::defer_lock};

            public:
                template <class... Args>
                explicit Mutexed(Args&&... args) : value{std::forward<Args>(args)...} {}

                Mutexed(Mutexed&&) noexcept = default;
                Mutexed& operator=(Mutexed&&) noexcept = default;

                Mutexed(const Mutexed&) noexcept = delete;
                Mutexed& operator=(const Mutexed&) noexcept = delete;

                /**
                 * Execute a function as a safely locked transaction
                 * @tparam F being the type of the callable for the transaction
                 * @param f being the transaction
                 * @return The result of the transaction
                 */
                template <class F>
                auto transaction(F&& f)
                    -> decltype(std::forward<F>(f)(static_cast<value_type&>(value))) {
                    SCL_MAYBE_UNUSED guard_type g{lock};
                    return std::forward<F>(f)(static_cast<value_type&>(value));
                }
        };

        template <class T, class Lock, class Guard>
        struct with_traits<Mutexed<T, Lock, Guard>> {
                template <class F>
                auto operator()(Mutexed<T, Lock, Guard>& mutexed, F&& delegate)
                    -> SCL_RETURN(mutexed.transaction(std::forward<F>(delegate)))
        };
    }  // namespace async
}  // namespace scl
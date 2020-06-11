#pragma once
#include <mutex>
#include <scl/async/with_traits.h>

namespace scl{
	namespace async{
		/**
		 * A wrapper class that protects a value with a lock
		 * @tparam T being the type of value to guard
		 * @tparam Lock being the lock type
		 * @tparam Guard being the type of guard to use on the lock
		 */
		template <class T, class Lock = std::mutex, template<class> class Guard = std::lock_guard>
		class Mutexed{
			public:
				using lock_type = Lock;
				using guard_type = Guard<lock_type>;
				using value_type = T;

			protected:
				value_type value;
				mutable std::unique_lock<lock_type> lock{lock_type{}, std::defer_lock};

			public:
				template <class... Args>
				explicit Mutexed(Args&&... args) : value{std::forward<Args>(args)...} {
				}

				Mutexed(Mutexed&&) noexcept = default;
				Mutexed& operator=(Mutexed&&) noexcept = default;

				Mutexed(const Mutexed&) = delete;
				Mutexed& operator=(const Mutexed&) = delete;

				template <class F>
				Mutexed& transaction(F&& f){
					{
						guard_type _{lock};
						std::forward<F>(f)(static_cast<value_type&>(this->value));
					}
					return *this;
				}
		};

		template <class T>
		struct with_traits<Mutexed<T>>{
			template <class F>
			auto operator()(Mutexed<T>& mutex, F&& delegate) -> META::return_t<F> {
				return mutex.transaction(std::forward<F&&>(delegate));
			}
		};
	}
}
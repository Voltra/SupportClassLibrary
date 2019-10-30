#pragma once
#include <scl/macros.h>
#include <scl/tools/meta/type_mod.h>
#include <scl/tools/meta/fn_introspect.h>
#include <mutex>
#include <scl/exceptions/InvalidResource.h>

namespace scl{
	namespace async{
		/**
		 * Traits for use with the scl::async::with function
		 * @tparam Resource being the type of the value passed to scl::async::with
		 */
		template <class Resource>
		struct with_traits{
			/**
			 * Invoke a delegate using the wrapper
			 * @tparam F being the type of the delegate
			 * @param resource being the resource to use
			 * @param delegate being the delegate to which the value will be passed
			 */
			template <class F>
			auto operator()(Resource& resource, F&& delegate) -> META::return_t<F> {
				throw scl::exceptions::InvalidResource{};
			}
		};

		template<>
		struct with_traits<std::mutex>{
			template <class F>
			auto operator()(std::mutex& mutex, F&& delegate) -> META::return_t<F> {
				std::lock_guard<decltype(mutex)> _{mutex};
				return std::forward<F>(delegate)();
			}
		};
	}
}
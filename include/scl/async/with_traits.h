#pragma once
#include <scl/macros.h>
#include <scl/tools/meta/type_mod.h>
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
			void operator()(Resource& resource, F&& delegate){
				throw scl::exceptions::InvalidResource{};
			}
		};

		template<>
		struct with_traits<std::mutex>{
			template <class F>
			void operator()(std::mutex& mutex, F&& delegate){
				std::lock_guard<decltype(mutex)> _{mutex};
				std::forward<F>(delegate)();
			}
		};
	}
}
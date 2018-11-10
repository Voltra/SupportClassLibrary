#pragma once

#include <utility>
#include <future>

namespace scl{
	namespace tools{
		namespace make{
			/**
			 * Creates a fulfilled std::promise in-place with the given set of arguments
			 * @tparam T being the value type of the fulfilled std::promise to create
			 * @tparam Args being the arguments' types
			 * @param args being the arguments to pass to the constructor of the value type
			 * @return a fulfilled std::promise<T> constructed using the given arguments
			 */
			template <class T, class... Args>
			inline std::promise<T> promise(Args&&... args){
				auto ret = std::promise<T>{};
				ret.set_value(T{std::forward<Args>(args)...});
				return ret;
			}
		}
	}
}
#pragma once

#include <utility>

namespace scl{
	namespace tools{
		namespace make{
			/**
			 * Instantiate the given value type in-place with the given set of arguments
			 * @tparam T being the value type to instantiate
			 * @tparam Args being the arguments' types
			 * @param args being the arguments to pass to the constructor of the value type
			 * @return a T constructed using the given arguments
			 */
			template <class T, class... Args>
			inline T from(Args&&... args){
				return T{std::forward<Args>(args)...};
			}
		}
	}
}
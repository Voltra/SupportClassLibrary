#pragma once

#include <utility>

namespace scl{
	namespace tools{
		namespace make{
			/**
			 * Creates a pointer in-place with the given set of arguments (using new)
			 * @tparam T being the value type of the pointer to create
			 * @tparam Args being the arguments' types
			 * @param args being the arguments to pass to the constructor of the value type
			 * @return a T* constructed using the given arguments
			 */
			template <class T, class... Args>
			inline T* ptr(Args&&... args){
				return new T(std::forward<Args>(args)...);
			}
		}
	}
}
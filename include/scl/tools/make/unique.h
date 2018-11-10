#pragma once

#include <memory>
#include "ptr.h"

namespace scl{
	namespace tools{
		/**
		 * Creates a unique_ptr in-place with the given set of arguments
		 * @tparam T being the value type of the unique_ptr to create
		 * @tparam Args being the arguments' types
		 * @param args being the arguments to pass to the constructor of the value type
		 * @return a std::unique_ptr<T> constructed using the given arguments
		 */
		namespace make{
			template <class T, class... Args>
			std::unique_ptr<T> unique(Args&&... args){
				return std::unique_ptr<T>{
					make::ptr<T>(std::forward<Args>(args)...)
				};
			}
		}
	}
}
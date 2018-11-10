#pragma once

#include <memory>
#include "ptr.h"

namespace scl{
	namespace tools{
		namespace make{
			/**
			 * Creates a shared_ptr in-place with the given set of arguments
			 * @tparam T being the value type of the shared_ptr to create
			 * @tparam Args being the arguments' types
			 * @param args being the arguments to pass to the constructor of the value type
			 * @return a std::shared_ptr<T> constructed using the given arguments
			 */
			template <class T, class... Args>
			std::shared_ptr<T> shared(Args&&... args){
				return std::shared_ptr<T>{
					make::ptr<T>(std::forward<Args>(args)...)
				};
			}
		}
	}
}
#pragma once

#include <utility>
#include <atomic>
#include "from.h"

namespace scl{
	namespace tools{
		namespace make{
			/**
			 * Creates an atomic in-place with the given set of arguments
			 * @tparam T being the value type of the atomic to create
			 * @tparam Args being the arguments' types
			 * @param args being the arguments to pass to the constructor of the value type
			 * @return a std::atomic<T> constructed using the given arguments
			 */
			template <class T, class... Args>
			inline std::atomic<T> atomic(Args&&... args){
				return std::atomic<T>{make::from(std::forward<Args>(args)...)};
			}
		}
	}
}
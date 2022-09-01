#pragma once
#include "../async/Mutexed.h"

namespace scl {
    namespace make {
	/**
	 * Creates an scl::async::Mutexed from the given type and arguments
	 * @tparam T being the type of values wrapped by the Mutexed
	 * @tparam Args being the types of the arguments passed to the constructor
	 * @param args being the arguments passed to the constructor
	 * @return an initialized Mutexed
	 */
	template <class T, class Lock = std::mutex, class Guard = std::lock_guard<Lock>,
		  class... Args>
	scl::async::Mutexed<T> mutexed(Args&&... args) {
	    return scl::async::Mutexed<T>{std::forward<Args>(args)...};
	}
    }  // namespace make
}  // namespace scl
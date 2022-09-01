#pragma once
#include <atomic>

namespace scl {
    namespace make {
	template <class T, class... Args>
	std::atomic<T> atomic(Args&&... args) {
	    return std::atomic<T>(std::forward<Args>(args)...);
	}
    }  // namespace make
}  // namespace scl
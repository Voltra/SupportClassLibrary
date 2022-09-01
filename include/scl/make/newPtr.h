#pragma once
#include <utility>

namespace scl {
    namespace make {
	/**
	 * Make a pointer using `new`
	 * @tparam Base
	 * @tparam Derived
	 * @tparam Args
	 * @param args
	 * @return
	 */
	template <class Base, class Derived = Base, class... Args>
	Base* newPtr(Args&&... args) {
	    return new Derived{std::forward<Args>(args)...};
	}
    }  // namespace make
}  // namespace scl
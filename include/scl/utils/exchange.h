#pragma once
#include "../meta/type_queries/stl.h"

namespace scl {
    namespace utils {
	template <class T, class U = T>
	constexpr T exchange(T& obj,
			     U&& newValue) noexcept(scl::meta::is_nothrow_move_constructible<T>()
						    && scl::meta::is_nothrow_assignable<T, U>()) {
	    T old = std::move(obj);
	    obj = std::forward<U>(newValue);
	    return old;
	}
    }  // namespace utils
}  // namespace scl
#pragma once

#include <scl/macros.h>
#include <scl/tools/meta/exists.h>

namespace scl{
	namespace concepts{
		/**
		 * AssignableWith concept, a type U is assignable to a type T if operator=() is defined for both types (or implicit conversions)
		 * @tparam T being the assignee's type
		 * @tparam U being the assigned's type
		 */
		template <class T, class U>
		struct AssignableWith{
			constexpr operator bool() const{
				static_assert(META::exists<decltype(
					std::declval<T>() = std::declval<U>()
				)>(), "AssignableWith<T,U>: Should be able to T = U");
				return true;
			}
		};
	}
}
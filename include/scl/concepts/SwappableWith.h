#pragma once

#ifdef SCL_CPP17
#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		/**
		 * SwappableWith concept, a type T is swappable with U if it defines swap semantics between the two
		 * @tparam T being the left hand side operand type
		 * @tparam U being the right hand side operand types
		 */
		template <class T, class U>
		struct SwappableWith{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_swappable_with<T, U>(), "SwappableWith<T, U>: Cannot swap a T and a U.");
				return true;
			}
		};
	}
}
#endif
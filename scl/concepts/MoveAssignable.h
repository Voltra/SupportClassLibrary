#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		/**
		 * MoveAssignable concept, a type T is move assignable if meta::is_move_assignable<T>() is true
		 * @tparam T the type to check against
		 */
		template <class T>
		struct MoveAssignable{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_move_assignable<T>(), "T is not move assignable");
				return true;
			}
		};
	}
}
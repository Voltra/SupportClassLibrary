#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
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
#pragma once

#include "../macros.h"
#include "../tools/meta/type_check.h"
//#include <scl/concepts/require.h>
#include "Destructible.h"

namespace scl{
	namespace concepts{
		/**
		 * MoveConstructible concept, a type T is move constructible if meta::is_move_constructible<T>() is true
		 * @tparam T the type to check against
		 */
		template <class T>
		struct MoveConstructible{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_require(Destructible<T>{});
				static_assert(meta::is_move_constructible<T>(), "MoveConstructible<T>: T is not move constructible");
				return true;
			}
		};
	}
}
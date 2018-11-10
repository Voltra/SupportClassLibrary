#pragma once

//#include <scl/concepts/require.h>
#include "../macros.h"
#include "../tools/meta/constexpr_assert.h"
#include "MoveAssignable.h"
#include "MoveConstructible.h"

namespace scl{
	namespace concepts{
		/**
		 * Movable concept, a type is movable if it is both move constructible and move assignable
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Movable{
			constexpr operator bool() const{
				return META::constexpr_assert<
					MoveConstructible<T>{}
					&& MoveAssignable<T>{}
				>();
			}
		};
	}
}
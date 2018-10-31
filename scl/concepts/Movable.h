#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/MoveAssignable.h>
#include <scl/concepts/MoveConstructible.h>

namespace scl{
	namespace concepts{
		/**
		 * Movable concept, a type is movable if it is both move constructible and move assignable
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Movable{
			constexpr operator bool() const{
				require(MoveConstructible<T>{});
				require(MoveAssignable<T>{});
				return true;
			}
		};
	}
}
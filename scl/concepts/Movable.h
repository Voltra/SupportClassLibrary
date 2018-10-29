#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/MoveAssignable.h>
#include <scl/concepts/MoveConstructible.h>

namespace scl{
	namespace concepts{
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
#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/Copyable.h>
#include <scl/concepts/DefaultConstructible.h>

namespace scl{
	namespace concepts{
		/**
		 * SemiRegular concept, a type is semi regular if it is Copyable and DefaultConstructible
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct SemiRegular{
			constexpr operator bool() const{
				require(Copyable<T>{});
				require(DefaultConstructible<T>{});
				return true;
			}
		};
	}
}
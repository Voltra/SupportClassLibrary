#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/Integral.h>
#include <scl/concepts/Unsigned.h>

namespace scl{
	namespace concepts{
		/**
		 * UnsignedIntegral concept, a type is an unsigned integral if it is both Integral and Unsigned
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct UnsignedIntegral{
			constexpr operator bool() const{
				require(Integral<T>{});
				require(Unsigned<T>{});
				return true;
			}
		};
	}
}
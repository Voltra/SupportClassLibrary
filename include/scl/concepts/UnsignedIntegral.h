#pragma once

//#include <scl/concepts/require.h>
#include <scl/macros.h>
#include <scl/tools/meta/constexpr_assert.h>
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
				return META::constexpr_assert<Integral<T>{} && Unsigned<T>{}>();
			}
		};
	}
}
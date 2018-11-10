#pragma once

//#include <scl/concepts/require.h>
#include "../macros.h"
#include "../tools/meta/constexpr_assert.h"
#include "Integral.h"
#include "Signed.h"

namespace scl{
	namespace concepts{
		/**
		 * SignedIntegral concept, a type is a signed integral if it is both Integral and Signed
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct SignedIntegral{
			constexpr operator bool() const{
				return META::constexpr_assert<Integral<T>{} && Signed<T>{}>();
			}
		};
	}
}
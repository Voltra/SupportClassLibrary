#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/Integral.h>
#include <scl/concepts/Signed.h>

namespace scl{
	namespace concepts{
		/**
		 * SignedIntegral concept, a type is a signed integral if it is both Integral and Signed
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct SignedIntegral{
			constexpr operator bool() const{
				require(Integral<T>{});
				require(Signed<T>{});
				return true;
			}
		};
	}
}
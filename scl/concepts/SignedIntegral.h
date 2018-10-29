#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/Integral.h>
#include <scl/concepts/Signed.h>

namespace scl{
	namespace concepts{
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
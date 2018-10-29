#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/Integral.h>
#include <scl/concepts/Unsigned.h>

namespace scl{
	namespace concepts{
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
#pragma once

#ifdef SCL_CPP17
#include <scl/concepts/require.h>
#include <scl/concepts/Invocable.h>

namespace scl{
	namespace concepts{
		template <class F, class... Args>
		struct RegularInvocable{
			constexpr operator bool() const{
				require(Invocable<F, Args...>{});
				return true;
			}
		};
	}
}
#endif
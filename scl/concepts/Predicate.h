#pragma once

#ifdef SCL_CPP17
#include <scl/concepts/require.h>
#include <scl/concepts/RegularInvocable.h>
#include <scl/concepts/Boolean.h>

namespace scl{
	namespace concepts{
		template <class F, class... Args>
		struct Predicate{
			constexpr operator bool() const{
				using namespace scl::tools;
				require(RegularInvocable<F, Args...>{});
				require(Boolean<meta::invoke_result_t<F, Args...>>{});
				return true;
			}
		};
	}
}
#endif

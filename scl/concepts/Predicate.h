#pragma once

#ifdef SCL_CPP17
#include <scl/concepts/require.h>
#include <scl/concepts/RegularInvocable.h>
#include <scl/concepts/Boolean.h>

namespace scl{
	namespace concepts{
		/**
		 * Predicate concept, a type F is a predicate with Args if it is RegularInvocable and its result is Boolean
		 * @tparam F being the function type
		 * @tparam Args being the arguents' types
		 */
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

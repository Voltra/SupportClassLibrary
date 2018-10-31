#pragma once

#ifdef SCL_CPP17
#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		/**
		 * Invocable concept, a type F is invocable with Args if meta::is_invocable<F, Args...>() is true
		 * @tparam F being the type to check against
		 * @tparam Args being the arguments' types to check against
		 */
		template <class F, class... Args>
		struct Invocable{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_invocable<F, Args...>(), "Cannot invoke F w/ Args...");
				return true;
			}
		};
	}
}
#endif
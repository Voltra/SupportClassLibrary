#pragma once

#ifdef SCL_CPP17
#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
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
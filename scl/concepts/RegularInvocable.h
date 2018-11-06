#pragma once

//#ifdef SCL_CPP17
//#include <scl/concepts/require.h>
#include <scl/macros.h>
#include <scl/tools/meta/constexpr_assert.h>
#include <scl/concepts/Invocable.h>

namespace scl{
	namespace concepts{
		/**
		 * RegularInvocable concept, a type F is regular invocable with Args if it is Invocable
		 * and is equality preserving and does not modify its arguments
		 * @tparam F being the function type
		 * @tparam Args being the arguments' types
		 */
		template <class F, class... Args>
		struct RegularInvocable{
			constexpr operator bool() const{
				return META::constexpr_assert<Invocable<F, Args...>{}>();
			}
		};
	}
}
//#endif
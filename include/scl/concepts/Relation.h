#pragma once

//#ifdef SCL_CPP17
//#include <scl/concepts/require.h>
#include "../macros.h"
#include "../tools/meta/constexpr_assert.h"
#include "Predicate.h"

namespace scl{
	namespace concepts{
		/***
		 * Relation concept, a type R is a relation between T and U if it defines the 4 combinations
		 * @tparam R being the relation type
		 * @tparam T being the first argument type
		 * @tparam U being the last argument type
		 */
		template <class R, class T, class U>
		struct Relation{
			constexpr operator bool() const{
				using namespace scl::tools;
				return META::constexpr_assert<
					Predicate<R, T, T>{}
					&& Predicate<R, T, U>{}
					&& Predicate<R, U, T>{}
					&& Predicate<R, U, U>{}
				>();
			}
		};
	}
}
//#endif

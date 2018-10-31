#pragma once

#ifdef SCL_CPP17
#include <scl/concepts/require.h>
#include <scl/concepts/Predicate.h>

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
				require(Predicate<R, T, T>{});
				require(Predicate<R, T, U>{});
				require(Predicate<R, U, T>{});
				require(Predicate<R, U, U>{});
				return true;
			}
		};
	}
}
#endif

#pragma once

#ifdef SCL_CPP17
#include <scl/concepts/require.h>
#include <scl/concepts/Predicate.h>

namespace scl{
	namespace concepts{
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

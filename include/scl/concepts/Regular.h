#pragma once

//#include <scl/concepts/require.h>
#include "../macros.h"
#include "../tools/meta/constexpr_assert.h"
#include "SemiRegular.h"
#include "EqualityComparable.h"

namespace scl{
	namespace concepts{
		/**
		 * Regular concept, a type is Regular if it is SemiRegular and EqualityCOmparable
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Regular{
			constexpr operator bool() const{
				return META::constexpr_assert<
				    SemiRegular<T>{}
					&& EqualityComparable<T>{}
				>();
			}
		};
	}
}
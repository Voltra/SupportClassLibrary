#pragma once

//#include <scl/concepts/require.h>
#include "../macros.h"
#include "../tools/meta/constexpr_assert.h"
#include "Copyable.h"
#include "DefaultConstructible.h"

namespace scl{
	namespace concepts{
		/**
		 * SemiRegular concept, a type is semi regular if it is Copyable and DefaultConstructible
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct SemiRegular{
			constexpr operator bool() const{
				return META::constexpr_assert<Copyable<T>{} && DefaultConstructible<T>{}>();
			}
		};
	}
}
#pragma once

#include "../tools/meta/type_check.h"
#include "../tools/meta/type_mod.h"
#include "../tools/meta/constexpr_assert.h"
#include "../macros.h"
//#include <scl/concepts/require.h>
#include "Movable.h"
#include "ConvertibleTo.h"
#include "EqualityComparable.h"
#include "EqualityComparableWith.h"
#include "InequalityComparable.h"
#include "InequalityComparableWith.h"
#include "Destructible.h"

namespace scl{
	namespace concepts{
		/**
		 * Boolean concept, a type is boolean if it defines all boolean operations
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Boolean{
			constexpr operator bool() const{
				using namespace scl::tools;
				return meta::constexpr_assert<
					Movable<meta::remove_cv_t<T>>{}
					&& ConvertibleTo<meta::remove_reference_t<T>, bool>{}
					&& ConvertibleTo<decltype(!std::declval<T>()), bool>{}
					&& ConvertibleTo<decltype(std::declval<T>() && true), bool>{}
					&& ConvertibleTo<decltype(std::declval<T>() || true), bool>{}
					&& ConvertibleTo<decltype(std::declval<T>() && std::declval<T>()), bool>{}
					&& ConvertibleTo<decltype(std::declval<T>() || std::declval<T>()), bool>{}
					&& ConvertibleTo<decltype(true && std::declval<T>()), bool>{}
					&& ConvertibleTo<decltype(true || std::declval<T>()), bool>{}
					&& EqualityComparable<T>{}
					&& EqualityComparableWith<T, bool>{}
					&& EqualityComparableWith<bool, T>{}
					&& InequalityComparable<T>{}
					&& InequalityComparableWith<T, bool>{}
					&& InequalityComparableWith<bool, T>{}
					&& Destructible<T>{}
				>();
			}
		};
	}
}
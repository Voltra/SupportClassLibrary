#pragma once

#include <scl/tools/meta/type_check.h>
#include <scl/tools/meta/type_mod.h>
#include <scl/tools/meta/constexpr_assert.h>
#include <scl/macros.h>
//#include <scl/concepts/require.h>
#include <scl/concepts/Movable.h>
#include <scl/concepts/ConvertibleTo.h>
#include <scl/concepts/EqualityComparable.h>
#include <scl/concepts/EqualityComparableWith.h>
#include <scl/concepts/InequalityComparable.h>
#include <scl/concepts/InequalityComparableWith.h>
#include <scl/concepts/Destructible.h>

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
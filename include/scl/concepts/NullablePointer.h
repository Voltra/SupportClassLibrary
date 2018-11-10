#pragma once

#include "../macros.h"
#include "../tools/meta/constexpr_assert.h"
#include "../tools/meta/exists.h"
#include "../tools/meta/is_convertible.h"
#include "AssignableWith.h"
#include "Constructible.h"
#include "DefaultConstructible.h"
#include "InequalityComparable.h"
#include "EqualityComparableWith.h"
#include "InequalityComparableWith.h"

namespace scl{
	namespace concepts{
		/**
		 * NullablePointer concept, a type is said to be a nullable pointer type if its interactions
		 * w/ nullptr are defined and can be compared using operator!=
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct NullablePointer{
			constexpr operator bool() const{
				static_assert(AssignableWith<T, std::nullptr_t>{}, "");

				static_assert(META::is_convertible<decltype(
					std::declval<T>() = nullptr
				), T&>(), "NullablePointer<T>: Assignment should return a reference");

				return META::constexpr_assert<
					Constructible<T, std::nullptr_t>{}
					&& DefaultConstructible<T>{}
					&& InequalityComparable<T>{}
					&& EqualityComparableWith<T, std::nullptr_t>{}
					&& EqualityComparableWith<std::nullptr_t, T>{}
					&& InequalityComparableWith<T, std::nullptr_t>{}
					&& InequalityComparableWith<std::nullptr_t, T>{}
				>();
			}
		};
	}
}
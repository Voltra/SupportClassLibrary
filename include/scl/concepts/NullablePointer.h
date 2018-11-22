#pragma once

#include <scl/macros.h>
#include <scl/tools/meta/constexpr_assert.h>
#include <scl/tools/meta/exists.h>
#include <scl/tools/meta/is_convertible.h>
#include <scl/concepts/AssignableWith.h>
#include <scl/concepts/Constructible.h>
#include <scl/concepts/DefaultConstructible.h>
#include <scl/concepts/InequalityComparable.h>
#include <scl/concepts/EqualityComparableWith.h>
#include <scl/concepts/InequalityComparableWith.h>

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
#pragma once

#include "../tools/meta/is_convertible.h"

namespace scl{
	namespace concepts{
		/**
		 * LessThanComparable concept, a type T is less than comparable if it defines an operator< with itself
		 * @tparam T the type to check against
		 */
		template <class T>
		struct LessThanComparable{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_convertible<
					decltype(std::declval<T>() < std::declval<T>()),
					bool
				>(), "LessThanComparable<T>: operator< is not/poorly defined for T");
				return true;
			}
		};
	}
}
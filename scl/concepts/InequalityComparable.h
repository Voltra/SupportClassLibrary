#pragma once

#include <scl/tools/meta/is_convertible.h>

namespace scl{
	namespace concepts{
		/**
		 * InequalityComparable concept, a type T is equality comparable if it defines an operator!= with itself
		 * @tparam T the type to check against
		 */
		template <class T>
		struct InequalityComparable{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_convertible<
					decltype(std::declval<T>() != std::declval<T>()),
					bool
				>(), "operator== is not/poorly defined for T");
				return true;
			}
		};
	}
}
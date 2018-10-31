#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		/**
		 * Unsigned concept, a type T is unsigned if meta::is_unsigned<T>() is true
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Unsigned{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_unsigned<T>(), "T is not a unsigned type");
				return true;
			}
		};
	}
}
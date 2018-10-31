#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		/**
		 * Intergal concept, a type T is integral if meta:is_integral<T>() is true
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Integral{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_integral<T>(), "T is not an integral type");
				return true;
			}
		};
	}
}
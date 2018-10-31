#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		/**
		 * Signed concept, a type T is signed if meta::is_signed<T>() is true
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Signed{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_signed<T>(), "T is not a signed type");
				return true;
			}
		};
	}
}
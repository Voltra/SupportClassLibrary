#pragma once

#ifdef SCL_CPP17
#include <scl/tools/meta/type_query.h>

namespace scl{
	namespace concepts{
		/**
		 * Swappable concept, a type is swappable if it defines swapping mecanisms
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Swappable{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_swappable<T>(), "T does not implement swapping mecanisms.");
				return true;
			}
		};
	}
}
#endif
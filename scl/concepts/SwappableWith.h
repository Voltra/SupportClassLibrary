#pragma once

#ifdef SCL_CPP17
#include <scl/tools/meta/type_query.h>

namespace scl{
	namespace concepts{
		template <class T, class U>
		struct SwappableWith{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_swappable_with<T, U>(), "Cannot swap a T and a U.");
				return true;
			}
		};
	}
}
#endif
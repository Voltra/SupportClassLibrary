#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		/**
		 * Destructible concept, a type T is destructible if it defines a destructor that must not throw
		 * @tparam T the type to check against
		 */
		template <class T>
		struct Destructible{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_nothrow_destructible<T>(), "T is not destructible");
				return true;
			}
		};
	}
}
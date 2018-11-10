#pragma once

#include "../tools/meta/type_check.h"

namespace scl{
	namespace concepts{
		/**
		 * Destructible concept, a type T is destructible if it defines a destructor that won't throw
		 * @tparam T the type to check against
		 */
		template <class T>
		struct Destructible{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_nothrow_destructible<T>(), "Destructible<T>: T is not nothrow destructible");
				return true;
			}
		};
	}
}
#pragma once

#include "../tools/meta/type_check.h"
//#include <scl/concepts/require.h>
#include "../macros.h"
#include "Destructible.h"

namespace scl{
	namespace concepts{
		/**
		 * CopyConstructible concept, a type T is copy constructible if meta::is_copy_constructible<T>() is true
		 * @tparam T the type to check against
		 */
		template <class T>
		struct CopyConstructible{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_require(Destructible<T>{});
				static_assert(meta::is_copy_constructible<T>(), "CopyConstructible<T>: T is not copy constructible");
				return true;
			}
		};
	}
}
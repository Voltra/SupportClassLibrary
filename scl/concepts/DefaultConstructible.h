#pragma once

#include <scl/tools/meta/type_check.h>
#include <scl/concepts/require.h>
#include <scl/concepts/Destructible.h>

namespace scl{
	namespace concepts{
		/**
		 * DefaultConstructible concept, a type T is default constructible if it defines a default constructor
		 * @tparam T the type to check against
		 */
		template <class T>
		struct DefaultConstructible{
			constexpr operator bool() const{
				using namespace scl::tools;
				require(Destructible<T>{});
				static_assert(meta::is_default_constructible<T>(), "T is not default constructible");
				return true;
			}
		};
	}
}
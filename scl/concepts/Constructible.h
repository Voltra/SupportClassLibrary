#pragma once

#include <scl/tools/meta/type_check.h>
//#include <scl/concepts/require.h>
#include <scl/macros.h>
#include <scl/concepts/Destructible.h>

namespace scl{
	namespace concepts{
		/**
		 * Constructible concept, a type is constructible with the given arguments
		 * if it is destructible and meta::is_constructible<T, Args...>() is true
		 * @tparam T being the type of object to construct
		 * @tparam Args being the types of the arguments to use in the constructor
		 */
		template <class T, class... Args>
		struct Constructible{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_require(Destructible<T>{});
				static_assert(meta::is_constructible<T, Args...>(), "T is not constructible w/ Args...");
				return true;
			}
		};
	}
}
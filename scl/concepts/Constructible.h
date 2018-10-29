#pragma once

#include <scl/tools/meta/type_check.h>
#include <scl/concepts/require.h>
#include <scl/concepts/Destructible.h>

namespace scl{
	namespace concepts{
		template <class T, class... Args>
		struct Constructible{
			constexpr operator bool() const{
				using namespace scl::tools;
				require(Destructible<T>{});
				static_assert(meta::is_constructible<T, Args...>(), "T is not constructible via Args...");
				return true;
			}
		};
	}
}
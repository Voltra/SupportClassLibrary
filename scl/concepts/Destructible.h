#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
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
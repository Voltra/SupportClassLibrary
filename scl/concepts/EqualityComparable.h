#pragma once

#include <scl/tools/meta/is_convertible.h>

namespace scl{
	namespace concepts{
		template <class T>
		struct EqualityComparable{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_convertible<
					decltype(std::declval<T>() == std::declval<T>()),
					bool
				>(), "operator== is not/poorly defined for T");
				return true;
			}
		};
	}
}
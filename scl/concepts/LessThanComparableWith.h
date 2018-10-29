#pragma once

#include <scl/tools/meta/is_convertible.h>

namespace scl{
	namespace concepts{
		template <class T, class U>
		struct LessThanComparableWith{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_convertible<
					decltype(std::declval<T>() < std::declval<U>()),
					bool
				>(), "There is no operator< that compares a T and a U.");
				return true;
			}
		};
	}
}
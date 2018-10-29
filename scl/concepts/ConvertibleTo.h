#pragma once

#include <scl/tools/meta/is_convertible.h>

namespace scl{
	namespace concepts{
		template <class From, class To>
		struct ConvertibleTo{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_convertible<From, To>(), "Requirements for concept ConvertibleTo are not met.");
				return true;
			}
		};
	}
}
#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		template <class T>
		struct Integral{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_integral<T>(), "T is not an integral type");
				return true;
			}
		};
	}
}
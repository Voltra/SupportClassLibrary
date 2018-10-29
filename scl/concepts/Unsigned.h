#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		template <class T>
		struct Unsigned{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_unsigned<T>(), "T is not a unsigned type");
				return true;
			}
		};
	}
}
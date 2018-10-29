#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		template <class T>
		struct CopyAssignable{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_copy_assignable<T>(), "T is not copy assignable");
				return true;
			}
		};
	}
}
#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		/**
		 * CopyAssignable concept, a type T is copy assignable if meta::is_copy_assignable<T>() is true
		 * @tparam T the type to check against
		 */
		template <class T>
		struct CopyAssignable{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(meta::is_copy_assignable<T>(), "CopyAssignable<T>: T is not copy assignable");
				return true;
			}
		};
	}
}
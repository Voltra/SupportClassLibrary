#pragma once

#include <scl/tools/meta/type_check.h>

namespace scl{
	namespace concepts{
		/**
		 * NonCopyable concept, a type is non copyable if it is neither copy constructible nor copy assignable
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct NonCopyable{
			constexpr operator bool() const{
				using namespace scl::tools;
				static_assert(!meta::is_copy_constructible<T>(), "T is copy constructible");
				static_assert(!meta::is_copy_assignable<T>(), "T is copy assignable");
				return true;
			}
		};
	}
}
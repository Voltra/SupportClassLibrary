#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/CopyAssignable.h>
#include <scl/concepts/CopyConstructible.h>

namespace scl{
	namespace concepts{
		/**
		 * Copyable concept, a type is copyable if it is both copy constructible and copy assignable
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Copyable{
			constexpr operator bool() const{
				require(CopyConstructible<T>{});
				require(CopyAssignable<T>{});
				return true;
			}
		};
	}
}
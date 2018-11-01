#pragma once

//#include <scl/concepts/require.h>
#include <scl/macros.h>
#include <scl/tools/meta/constexpr_assert.h>
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
				return META::constexpr_assert<CopyConstructible<T>{} && CopyAssignable<T>{}>();
			}
		};
	}
}
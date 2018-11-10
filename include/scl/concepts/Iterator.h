#pragma once

#include "../macros.h"
#include "require.h"
#include "Copyable.h"
#include "Destructible.h"
#include "Swappable.h"
#include "../tools/meta/type_mod.h"
#include "../tools/meta/constexpr_assert.h"
#include "../tools/meta/exists.h"
#include "../tools/meta/is_convertible.h"
#include <iterator>

namespace scl{
	namespace concepts{
		/**
		 * Iterator concept, a type is an iterator if it is copyable, destructible, swappable,
		 * has well-defined iterator_traits and can be incremented and dereferenced
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct Iterator{
			constexpr operator bool() const{
				return META::constexpr_assert<
					Copyable<T>{}
					&& Destructible<T>{}
//#ifdef SCL_CPP17
					&& Swappable<T>{}
//#endif
#define SCL_TRAIT(type) typename std::iterator_traits<T>::type
					&& META::exists<SCL_TRAIT(value_type)>()
					&& META::exists<SCL_TRAIT(difference_type)>()
					&& META::exists<SCL_TRAIT(reference)>()
					&& META::exists<SCL_TRAIT(pointer)>()
					&& META::exists<SCL_TRAIT(iterator_category)>()
#undef SCL_TRAIT
#define SCL_LVALUE std::declval<T&>()
					&& META::exists<decltype(*SCL_LVALUE)>()
					&& META::is_convertible<decltype(++SCL_LVALUE), T&>()
#undef SCL_LVALUE
				>();
			}
		};
	}
}
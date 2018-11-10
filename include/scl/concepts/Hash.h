#pragma once

#include "../macros.h"
#include "FunctionObject.h"
#include "CopyConstructible.h"
#include "Destructible.h"
#include "../tools/meta/is_same.h"

namespace scl{
	namespace concepts{
		/**
		 * Hash concept, a type T is an hash for Key if it is a function object, copy constructible, destructible,
		 * and the return type of its invocation w/ a Key is std::size_t
		 * @tparam T being the type to check against
		 * @tparam Key being the type to hash
		 */
		template <class T, class Key>
		struct Hash{
			constexpr operator bool() const{
				return FunctionObject<T, Key>{}
				&& CopyConstructible<T>{}
				&& Destructible<T>{}
				&& META::is_same<
					decltype(std::declval<T>()(std::declval<Key>())), //call of T(Key)
					std::size_t
				>();
			}
		};
	}
}
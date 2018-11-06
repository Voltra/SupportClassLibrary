#pragma once

#include <scl/concepts/require.h>
#include <scl/concepts/Iterator.h>
#include <scl/concepts/Swappable.h>
#include <iterator>

#define SCL_TRAIT(type) typename std::iterator_traits<T>::type

namespace scl{
	namespace concepts{
		/**
		 * ValueSwappable concept, a type is value swappable if it is an iterator type and its value type is swappable
		 * @tparam T being the type to check against
		 */
		template <class T>
		struct ValueSwappable{
			constexpr operator bool() const{
				require(Iterator<T>{});
				require(Swappable<SCL_TRAIT(value_type)>{});
				return true;
			}
		};
	}
}
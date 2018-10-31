#pragma once

#include <type_traits>
#include <scl/tools/meta/void_t.h>

namespace scl{
	namespace tools{
		namespace meta{
			template <class T, class = void>
			struct __exists : std::false_type{};

			template <class T>
			struct __exists<T, void_t<T>> : std::true_type{};

			/**
			 * Determines whether or not the given type is well-formed
			 * @tparam T being the type to check
			 * @return TRUE if well formed, FALSE otherwise
			 */
			template <class T>
			inline constexpr bool exists(){
				return __exists<T>::value;
			}
		}
	}
}
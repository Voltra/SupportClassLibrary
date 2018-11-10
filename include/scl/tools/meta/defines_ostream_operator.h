#pragma once

#include <type_traits>
#include <iostream>
#include <string>
#include "void_t.h"
#include "exists.h"
#include "enable_if.h"
#include "type_mod.h"

namespace scl{
	namespace tools{
		namespace meta{
			template <class T, class = void>
			struct __ostream_op_defined : std::false_type{};

			template <class T>
			struct __ostream_op_defined<T, /*enable_if_t*/void_t<
				/*exists<*/decltype(operator<<(std::declval<std::ostream&>(), std::declval<const T&>()))/*>()*/
			>> : std::true_type{};

			/**
			 * Determines whether or not the operator<< is defined for the given type and ostream
			 * @tparam T being the type to test upon
			 * @return TRUE if defined, FALSE otherwise
			 */
			template <class T>
			inline constexpr bool defines_ostream_operator(){
				return __ostream_op_defined<decay_t<T>>::value;
			}
		}
	}
}
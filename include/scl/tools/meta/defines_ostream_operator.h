#pragma once

#include <type_traits>
#include <iostream>
#include <string>
#include <scl/tools/meta/void_t.h>
#include <scl/tools/meta/exists.h>
#include <scl/tools/meta/enable_if.h>
#include <scl/tools/meta/type_mod.h>

namespace scl{
	namespace tools{
		namespace meta{
			namespace details{
				template <class T, class = void>
				struct __ostream_op_defined : std::false_type{};

				template <class T>
				struct __ostream_op_defined<T, /*enable_if_t*/void_t<
															  /*exists<*/decltype(operator<<(std::declval<std::ostream&>(), std::declval<const T&>()))/*>()*/
				>> : std::true_type{};
			}

			/**
			 * Determines whether or not the operator<< is defined for the given type and ostream
			 * @tparam T being the type to test upon
			 * @return TRUE if defined, FALSE otherwise
			 */
			template <class T>
			inline constexpr bool defines_ostream_operator(){
				return details::__ostream_op_defined<decay_t<T>>::value;
			}
		}
	}
}